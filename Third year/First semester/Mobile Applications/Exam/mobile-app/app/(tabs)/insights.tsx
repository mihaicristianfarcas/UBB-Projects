import React, { useState, useEffect, useMemo, useCallback } from "react";
import { View, StyleSheet, FlatList } from "react-native";
import NetInfo from "@react-native-community/netinfo";
import { Ionicons } from "@expo/vector-icons";
import { useFocusEffect } from "@react-navigation/native";
import { getAllSales } from "@/utils/api";
import { saveAllSales, getLocalAllSales } from "@/utils/storage";
import { Transaction } from "@/types/transaction";
import { log } from "@/utils/logger";
import LoadingSpinner from "@/components/loading-spinner";
import { ThemedText } from "@/components/themed-text";

interface CategoryTotal {
	category: string;
	total: number;
}

const MEDAL_COLORS = ["#FFD700", "#C0C0C0", "#CD7F32"];
const MEDAL_ICONS = ["trophy", "medal", "ribbon"] as const;

export default function InsightsScreen() {
	const [sales, setSales] = useState<Transaction[]>([]);
	const [loading, setLoading] = useState(false);
	const [isOnline, setIsOnline] = useState(false);
	const [isCached, setIsCached] = useState(false);

	useEffect(() => {
		const unsubscribe = NetInfo.addEventListener((state) => {
			const online = state.isConnected ?? false;
			setIsOnline(online);
			log(`Network: ${online ? "Online" : "Offline"}`, "info");
		});
		NetInfo.fetch().then((state) => setIsOnline(state.isConnected ?? false));
		return () => unsubscribe();
	}, []);

	const loadSales = useCallback(async () => {
		const cached = await getLocalAllSales();
		if (cached.length > 0) {
			setSales(cached);
			setIsCached(true);
		}

		if (!isOnline) return;

		setLoading(true);
		setIsCached(false);
		const response = await getAllSales();
		setLoading(false);

		if (response.error) {
			log(`Error loading sales: ${response.error}`, "error");
			return;
		}

		if (response.data) {
			await saveAllSales(response.data);
			setSales(response.data);
			setIsCached(false);
		}
	}, [isOnline]);

	useFocusEffect(
		useCallback(() => {
			loadSales();
		}, [loadSales]),
	);

	const topCategories = useMemo(() => {
		const totals: Record<string, number> = {};

		for (const sale of sales) {
			totals[sale.category] = (totals[sale.category] || 0) + sale.amount;
		}

		return Object.entries(totals)
			.map(([category, total]) => ({ category, total }))
			.sort((a, b) => b.total - a.total)
			.slice(0, 3);
	}, [sales]);

	const maxTotal = useMemo(() => {
		if (topCategories.length === 0) return 1;
		return Math.max(...topCategories.map((c) => c.total));
	}, [topCategories]);

	const renderCategoryItem = ({
		item,
		index,
	}: {
		item: CategoryTotal;
		index: number;
	}) => {
		const barWidth = (item.total / maxTotal) * 100;

		return (
			<View style={styles.categoryCard}>
				<View style={styles.cardHeader}>
					<View
						style={[
							styles.medalCircle,
							{ backgroundColor: MEDAL_COLORS[index] + "22" },
						]}
					>
						<Ionicons
							name={MEDAL_ICONS[index]}
							size={22}
							color={MEDAL_COLORS[index]}
						/>
					</View>
					<View style={styles.cardInfo}>
						<ThemedText type="defaultSemiBold" style={styles.categoryName}>
							{item.category.charAt(0).toUpperCase() + item.category.slice(1)}
						</ThemedText>
						<ThemedText style={styles.categoryTotal}>
							$
							{item.total.toLocaleString(undefined, {
								minimumFractionDigits: 2,
								maximumFractionDigits: 2,
							})}
						</ThemedText>
					</View>
					<View style={styles.rankLabel}>
						<ThemedText style={styles.rankLabelText}>#{index + 1}</ThemedText>
					</View>
				</View>
				<View style={styles.barContainer}>
					<View
						style={[
							styles.bar,
							{ width: `${barWidth}%`, backgroundColor: MEDAL_COLORS[index] },
						]}
					/>
				</View>
			</View>
		);
	};

	return (
		<View style={styles.container}>
			{isCached && (
				<View style={styles.offlineBanner}>
					<Ionicons name="cloud-offline-outline" size={18} color="#856404" />
					<ThemedText style={styles.offlineText}>
						Offline — Showing cached data
					</ThemedText>
				</View>
			)}

			{loading ? (
				<LoadingSpinner visible={true} message="Loading insights..." />
			) : topCategories.length === 0 ? (
				<View style={styles.emptyContainer}>
					<Ionicons name="bulb-outline" size={48} color="#C7C7CC" />
					<ThemedText style={styles.emptyText}>No sales data found</ThemedText>
				</View>
			) : (
				<FlatList
					data={topCategories}
					renderItem={renderCategoryItem}
					keyExtractor={(item) => item.category}
					contentContainerStyle={styles.listContent}
					ListHeaderComponent={
						<ThemedText type="subtitle" style={styles.sectionTitle}>
							Top 3 Property Categories
						</ThemedText>
					}
					ItemSeparatorComponent={() => <View style={styles.separator} />}
				/>
			)}
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
		paddingBottom: 32,
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
	sectionTitle: {
		color: "#1C1C1E",
		marginBottom: 16,
	},
	categoryCard: {
		backgroundColor: "#fff",
		borderRadius: 14,
		padding: 18,
		shadowColor: "#000",
		shadowOffset: { width: 0, height: 1 },
		shadowOpacity: 0.06,
		shadowRadius: 6,
		elevation: 2,
	},
	cardHeader: {
		flexDirection: "row",
		alignItems: "center",
		marginBottom: 14,
		gap: 14,
	},
	medalCircle: {
		width: 44,
		height: 44,
		borderRadius: 22,
		alignItems: "center",
		justifyContent: "center",
	},
	cardInfo: {
		flex: 1,
	},
	categoryName: {
		fontSize: 17,
		color: "#1C1C1E",
		marginBottom: 2,
	},
	categoryTotal: {
		fontSize: 15,
		fontWeight: "700",
		color: "#34C759",
	},
	rankLabel: {
		backgroundColor: "#F2F2F7",
		paddingHorizontal: 10,
		paddingVertical: 4,
		borderRadius: 8,
	},
	rankLabelText: {
		fontSize: 13,
		fontWeight: "700",
		color: "#8E8E93",
	},
	barContainer: {
		height: 6,
		backgroundColor: "#F2F2F7",
		borderRadius: 3,
		overflow: "hidden",
	},
	bar: {
		height: 6,
		borderRadius: 3,
	},
	separator: {
		height: 12,
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
});
