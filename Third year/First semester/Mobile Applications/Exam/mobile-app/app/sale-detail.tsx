import React, { useState, useEffect } from "react";
import { View, StyleSheet, Alert, ScrollView } from "react-native";
import { useLocalSearchParams } from "expo-router";
import NetInfo from "@react-native-community/netinfo";
import { Ionicons } from "@expo/vector-icons";
import { ThemedText } from "@/components/themed-text";
import LoadingSpinner from "@/components/loading-spinner";
import { getSaleById } from "@/utils/api";
import { saveSaleDetail, getLocalSaleDetail } from "@/utils/storage";
import { Transaction } from "@/types/transaction";
import { log } from "@/utils/logger";

export default function SaleDetailScreen() {
	const { id } = useLocalSearchParams<{ id: string }>();
	const [sale, setSale] = useState<Transaction | null>(null);
	const [loading, setLoading] = useState(true);

	useEffect(() => {
		loadSaleDetail();
	}, [id]);

	const loadSaleDetail = async () => {
		const saleId = Number(id);

		const netState = await NetInfo.fetch();
		const isOnline = netState.isConnected ?? false;

		if (isOnline) {
			setLoading(true);
			const response = await getSaleById(saleId);
			setLoading(false);

			if (response.error) {
				Alert.alert("Error", response.error);
				const cached = await getLocalSaleDetail(saleId);
				if (cached) setSale(cached);
				return;
			}

			if (response.data) {
				setSale(response.data);
				await saveSaleDetail(response.data);
			}
		} else {
			log("Offline - loading sale detail from cache", "info");
			const cached = await getLocalSaleDetail(saleId);
			setLoading(false);
			if (cached) {
				setSale(cached);
			} else {
				Alert.alert("Offline", "Sale details not available offline");
			}
		}
	};

	if (loading) {
		return (
			<View style={styles.container}>
				<LoadingSpinner visible={true} message="Loading sale details..." />
			</View>
		);
	}

	if (!sale) {
		return (
			<View style={styles.emptyContainer}>
				<Ionicons name="document-text-outline" size={48} color="#C7C7CC" />
				<ThemedText style={styles.emptyText}>Sale not found</ThemedText>
			</View>
		);
	}

	return (
		<ScrollView
			style={styles.container}
			contentContainerStyle={styles.scrollContent}
		>
			<View style={styles.card}>
				<View style={styles.cardHeader}>
					<ThemedText type="subtitle" style={styles.title}>
						Sale #{sale.id}
					</ThemedText>
					<View style={styles.typeBadge}>
						<ThemedText style={styles.typeBadgeText}>
							{sale.type.toUpperCase()}
						</ThemedText>
					</View>
				</View>

				<View style={styles.amountRow}>
					<ThemedText style={styles.amountLabel}>Amount</ThemedText>
					<ThemedText style={styles.amountValue}>
						${sale.amount.toLocaleString()}
					</ThemedText>
				</View>

				<View style={styles.divider} />

				<Field icon="calendar-outline" label="Date" value={sale.date} />
				<Field icon="pricetag-outline" label="Category" value={sale.category} />
				<Field
					icon="document-text-outline"
					label="Description"
					value={sale.description}
				/>
			</View>
		</ScrollView>
	);
}

function Field({
	icon,
	label,
	value,
}: {
	icon: keyof typeof Ionicons.glyphMap;
	label: string;
	value: string;
}) {
	return (
		<View style={styles.field}>
			<View style={styles.fieldIcon}>
				<Ionicons name={icon} size={18} color="#8E8E93" />
			</View>
			<View style={styles.fieldContent}>
				<ThemedText style={styles.fieldLabel}>{label}</ThemedText>
				<ThemedText style={styles.fieldValue}>{value}</ThemedText>
			</View>
		</View>
	);
}

const styles = StyleSheet.create({
	container: {
		flex: 1,
		backgroundColor: "#F2F2F7",
	},
	scrollContent: {
		padding: 16,
	},
	card: {
		backgroundColor: "#fff",
		borderRadius: 14,
		padding: 20,
		shadowColor: "#000",
		shadowOffset: { width: 0, height: 2 },
		shadowOpacity: 0.08,
		shadowRadius: 8,
		elevation: 3,
	},
	cardHeader: {
		flexDirection: "row",
		justifyContent: "space-between",
		alignItems: "center",
		marginBottom: 16,
	},
	title: {
		color: "#1C1C1E",
	},
	typeBadge: {
		backgroundColor: "#E8F0FE",
		paddingHorizontal: 12,
		paddingVertical: 5,
		borderRadius: 8,
	},
	typeBadgeText: {
		fontSize: 12,
		fontWeight: "700",
		color: "#007AFF",
		letterSpacing: 0.5,
	},
	amountRow: {
		backgroundColor: "#F2F2F7",
		borderRadius: 12,
		padding: 16,
		marginBottom: 16,
		alignItems: "center",
	},
	amountLabel: {
		fontSize: 13,
		color: "#8E8E93",
		marginBottom: 4,
	},
	amountValue: {
		fontSize: 28,
		fontWeight: "700",
		color: "#34C759",
	},
	divider: {
		height: 1,
		backgroundColor: "#F2F2F7",
		marginBottom: 16,
	},
	field: {
		flexDirection: "row",
		alignItems: "flex-start",
		marginBottom: 16,
		gap: 12,
	},
	fieldIcon: {
		width: 36,
		height: 36,
		borderRadius: 10,
		backgroundColor: "#F2F2F7",
		alignItems: "center",
		justifyContent: "center",
		marginTop: 2,
	},
	fieldContent: {
		flex: 1,
	},
	fieldLabel: {
		fontSize: 13,
		color: "#8E8E93",
		marginBottom: 2,
	},
	fieldValue: {
		fontSize: 16,
		color: "#1C1C1E",
	},
	emptyContainer: {
		flex: 1,
		backgroundColor: "#F2F2F7",
		alignItems: "center",
		justifyContent: "center",
		gap: 12,
	},
	emptyText: {
		fontSize: 16,
		color: "#8E8E93",
	},
});
