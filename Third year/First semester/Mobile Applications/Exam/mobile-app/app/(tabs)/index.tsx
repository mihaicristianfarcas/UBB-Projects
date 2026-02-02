import React, { useState, useEffect, useCallback } from "react";
import {
	View,
	StyleSheet,
	TouchableOpacity,
	Alert,
	FlatList,
	ActivityIndicator,
} from "react-native";
import { useRouter } from "expo-router";
import NetInfo from "@react-native-community/netinfo";
import { ThemedText } from "@/components/themed-text";
import LoadingSpinner from "@/components/loading-spinner";
import { getSales, deleteSale } from "@/utils/api";
import { saveSales, getLocalSales } from "@/utils/storage";
import { Transaction } from "@/types/transaction";
import { useWebSocket } from "@/hooks/use-web-socket";
import { log } from "@/utils/logger";
import { Ionicons } from "@expo/vector-icons";
import { useFocusEffect } from "expo-router";

export default function SalesScreen() {
	const router = useRouter();
	const [isOnline, setIsOnline] = useState(false);
	const [sales, setSales] = useState<Transaction[]>([]);
	const [loading, setLoading] = useState(false);
	const [deletingIds, setDeletingIds] = useState<Set<number>>(new Set());

	useEffect(() => {
		const unsubscribe = NetInfo.addEventListener((state) => {
			const online = state.isConnected ?? false;
			setIsOnline(online);
			log(`Network: ${online ? "Online" : "Offline"}`, "info");
		});
		NetInfo.fetch().then((state) => setIsOnline(state.isConnected ?? false));
		return () => unsubscribe();
	}, []);

	useFocusEffect(
		useCallback(() => {
			loadSales();
		}, [isOnline]),
	);

	useWebSocket({
		onMessage: (message: any) => {
			if (message.date && message.amount !== undefined && message.type) {
				Alert.alert(
					"New Sale Added",
					`Date: ${message.date}\nAmount: $${message.amount}\nType: ${message.type}\nCategory: ${message.category}`,
				);
				loadSales();
			}
		},
	});

	const loadSales = async () => {
		const cached = await getLocalSales();
		if (cached.length > 0) {
			setSales(cached);
		}

		if (!isOnline) return;

		setLoading(true);
		const response = await getSales();
		setLoading(false);

		if (response.error) {
			Alert.alert("Error", response.error);
			return;
		}

		if (response.data) {
			setSales(response.data);
			await saveSales(response.data);
		}
	};

	const handleDelete = (sale: Transaction) => {
		if (!isOnline) {
			Alert.alert("Offline", "Deleting sales requires an internet connection");
			return;
		}

		Alert.alert("Delete Sale", "Are you sure you want to delete this sale?", [
			{ text: "Cancel", style: "cancel" },
			{
				text: "Delete",
				style: "destructive",
				onPress: async () => {
					setDeletingIds((prev) => new Set(prev).add(sale.id));
					const response = await deleteSale(sale.id);
					setDeletingIds((prev) => {
						const next = new Set(prev);
						next.delete(sale.id);
						return next;
					});

					if (response.error) {
						Alert.alert("Error", response.error);
						return;
					}

					Alert.alert("Success", "Sale deleted");
					await loadSales();
				},
			},
		]);
	};

	const renderSale = ({ item }: { item: Transaction }) => {
		const isDeleting = deletingIds.has(item.id);

		return (
			<TouchableOpacity
				onPress={() =>
					router.push({ pathname: "/sale-detail", params: { id: item.id } })
				}
				activeOpacity={0.7}
			>
				<View style={styles.saleCard}>
					<View style={styles.saleHeader}>
						<View style={styles.typeBadge}>
							<ThemedText style={styles.typeBadgeText}>
								{item.type.toUpperCase()}
							</ThemedText>
						</View>
						<ThemedText style={styles.saleAmount}>
							${item.amount.toLocaleString()}
						</ThemedText>
					</View>
					<View style={styles.saleBody}>
						<ThemedText style={styles.saleDate}>{item.date}</ThemedText>
						<ThemedText style={styles.saleCategory}>{item.category}</ThemedText>
					</View>
					<ThemedText style={styles.saleDescription} numberOfLines={1}>
						{item.description}
					</ThemedText>
					<View style={styles.saleFooter}>
						<View style={styles.detailHint}>
							<ThemedText style={styles.detailHintText}>
								Tap for details
							</ThemedText>
							<Ionicons name="chevron-forward" size={14} color="#8E8E93" />
						</View>
						<TouchableOpacity
							style={[styles.deleteButton, isDeleting && styles.buttonDisabled]}
							onPress={() => handleDelete(item)}
							disabled={isDeleting}
						>
							{isDeleting ? (
								<ActivityIndicator size="small" color="#FF3B30" />
							) : (
								<Ionicons name="trash-outline" size={18} color="#FF3B30" />
							)}
						</TouchableOpacity>
					</View>
				</View>
			</TouchableOpacity>
		);
	};

	return (
		<View style={styles.container}>
			{!isOnline && (
				<View style={styles.offlineBanner}>
					<Ionicons name="cloud-offline-outline" size={18} color="#856404" />
					<ThemedText style={styles.offlineText}>
						Offline — Showing cached data
					</ThemedText>
					<TouchableOpacity onPress={loadSales} style={styles.retryButton}>
						<ThemedText style={styles.retryText}>Retry</ThemedText>
					</TouchableOpacity>
				</View>
			)}

			{loading ? (
				<LoadingSpinner visible={true} message="Loading sales..." />
			) : sales.length === 0 ? (
				<View style={styles.emptyContainer}>
					<Ionicons name="document-text-outline" size={48} color="#C7C7CC" />
					<ThemedText style={styles.emptyText}>No sales found</ThemedText>
				</View>
			) : (
				<FlatList
					data={sales}
					renderItem={renderSale}
					keyExtractor={(item) => item.id.toString()}
					contentContainerStyle={styles.listContent}
					ItemSeparatorComponent={() => <View style={styles.separator} />}
				/>
			)}

			<TouchableOpacity
				style={styles.fab}
				onPress={() => router.push("/add-sale")}
				activeOpacity={0.8}
			>
				<Ionicons name="add" size={28} color="#fff" />
			</TouchableOpacity>
		</View>
	);
}

const styles = StyleSheet.create({
	container: {
		flex: 1,
		backgroundColor: "#F2F2F7",
	},
	listContent: {
		padding: 16,
		paddingBottom: 100,
	},
	offlineBanner: {
		flexDirection: "row",
		alignItems: "center",
		backgroundColor: "#FFF3CD",
		paddingVertical: 10,
		paddingHorizontal: 16,
		gap: 8,
		borderBottomWidth: 1,
		borderBottomColor: "#FFE69C",
	},
	offlineText: {
		flex: 1,
		fontSize: 14,
		color: "#856404",
	},
	retryButton: {
		backgroundColor: "#FFC107",
		paddingHorizontal: 14,
		paddingVertical: 6,
		borderRadius: 6,
	},
	retryText: {
		fontSize: 13,
		fontWeight: "600",
		color: "#000",
	},
	saleCard: {
		backgroundColor: "#fff",
		borderRadius: 14,
		padding: 16,
		shadowColor: "#000",
		shadowOffset: { width: 0, height: 1 },
		shadowOpacity: 0.06,
		shadowRadius: 6,
		elevation: 2,
	},
	saleHeader: {
		flexDirection: "row",
		justifyContent: "space-between",
		alignItems: "center",
		marginBottom: 10,
	},
	typeBadge: {
		backgroundColor: "#E8F0FE",
		paddingHorizontal: 10,
		paddingVertical: 4,
		borderRadius: 6,
	},
	typeBadgeText: {
		fontSize: 11,
		fontWeight: "700",
		color: "#007AFF",
		letterSpacing: 0.5,
	},
	saleAmount: {
		fontSize: 20,
		fontWeight: "700",
		color: "#1C1C1E",
	},
	saleBody: {
		flexDirection: "row",
		justifyContent: "space-between",
		marginBottom: 6,
	},
	saleDate: {
		fontSize: 14,
		color: "#8E8E93",
	},
	saleCategory: {
		fontSize: 14,
		color: "#8E8E93",
		fontWeight: "500",
	},
	saleDescription: {
		fontSize: 14,
		color: "#636366",
		marginBottom: 12,
	},
	saleFooter: {
		flexDirection: "row",
		justifyContent: "space-between",
		alignItems: "center",
		borderTopWidth: 1,
		borderTopColor: "#F2F2F7",
		paddingTop: 10,
	},
	detailHint: {
		flexDirection: "row",
		alignItems: "center",
		gap: 2,
	},
	detailHintText: {
		fontSize: 13,
		color: "#8E8E93",
	},
	deleteButton: {
		padding: 8,
		borderRadius: 8,
		backgroundColor: "#FFF0F0",
	},
	buttonDisabled: {
		opacity: 0.5,
	},
	emptyContainer: {
		flex: 1,
		alignItems: "center",
		justifyContent: "center",
		gap: 12,
	},
	emptyText: {
		fontSize: 16,
		color: "#8E8E93",
	},
	separator: {
		height: 10,
	},
	fab: {
		position: "absolute",
		bottom: 24,
		right: 20,
		width: 56,
		height: 56,
		borderRadius: 28,
		backgroundColor: "#007AFF",
		alignItems: "center",
		justifyContent: "center",
		shadowColor: "#007AFF",
		shadowOffset: { width: 0, height: 4 },
		shadowOpacity: 0.3,
		shadowRadius: 8,
		elevation: 6,
	},
});
