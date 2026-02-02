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

interface MonthlyTotal {
	month: string;
	total: number;
}

export default function ReportsScreen() {
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

	const monthlyTotals = useMemo(() => {
		const totals: Record<string, number> = {};

		for (const sale of sales) {
			const month = sale.date.substring(0, 7);
			totals[month] = (totals[month] || 0) + sale.amount;
		}

		return Object.entries(totals)
			.map(([month, total]) => ({ month, total }))
			.sort((a, b) => b.total - a.total);
	}, [sales]);

	const formatMonth = (ym: string) => {
		const [year, month] = ym.split("-");
		const monthNames = [
			"January",
			"February",
			"March",
			"April",
			"May",
			"June",
			"July",
			"August",
			"September",
			"October",
			"November",
			"December",
		];
		const monthIndex = parseInt(month, 10) - 1;
		return `${monthNames[monthIndex]} ${year}`;
	};

	const maxTotal = useMemo(() => {
		if (monthlyTotals.length === 0) return 1;
		return Math.max(...monthlyTotals.map((m) => m.total));
	}, [monthlyTotals]);

	const renderMonthItem = ({
		item,
		index,
	}: {
		item: MonthlyTotal;
		index: number;
	}) => {
		const barWidth = (item.total / maxTotal) * 100;

		return (
			<View style={styles.monthCard}>
				<View style={styles.monthHeader}>
					<View style={styles.rankBadge}>
						<ThemedText style={styles.rankText}>#{index + 1}</ThemedText>
					</View>
					<ThemedText type="defaultSemiBold" style={styles.monthName}>
						{formatMonth(item.month)}
					</ThemedText>
					<ThemedText style={styles.monthTotal}>
						$
						{item.total.toLocaleString(undefined, {
							minimumFractionDigits: 2,
							maximumFractionDigits: 2,
						})}
					</ThemedText>
				</View>
				<View style={styles.barContainer}>
					<View style={[styles.bar, { width: `${barWidth}%` }]} />
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
				<LoadingSpinner visible={true} message="Loading reports..." />
			) : monthlyTotals.length === 0 ? (
				<View style={styles.emptyContainer}>
					<Ionicons name="bar-chart-outline" size={48} color="#C7C7CC" />
					<ThemedText style={styles.emptyText}>No sales data found</ThemedText>
				</View>
			) : (
				<FlatList
					data={monthlyTotals}
					renderItem={renderMonthItem}
					keyExtractor={(item) => item.month}
					contentContainerStyle={styles.listContent}
					ListHeaderComponent={
						<ThemedText type="subtitle" style={styles.sectionTitle}>
							Monthly Sales Analysis
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
	monthCard: {
		backgroundColor: "#fff",
		borderRadius: 14,
		padding: 16,
		shadowColor: "#000",
		shadowOffset: { width: 0, height: 1 },
		shadowOpacity: 0.06,
		shadowRadius: 6,
		elevation: 2,
	},
	monthHeader: {
		flexDirection: "row",
		alignItems: "center",
		marginBottom: 12,
		gap: 10,
	},
	rankBadge: {
		backgroundColor: "#F2F2F7",
		width: 32,
		height: 32,
		borderRadius: 16,
		alignItems: "center",
		justifyContent: "center",
	},
	rankText: {
		fontSize: 13,
		fontWeight: "700",
		color: "#8E8E93",
	},
	monthName: {
		flex: 1,
		fontSize: 16,
		color: "#1C1C1E",
	},
	monthTotal: {
		fontSize: 16,
		fontWeight: "700",
		color: "#34C759",
	},
	barContainer: {
		height: 6,
		backgroundColor: "#F2F2F7",
		borderRadius: 3,
		overflow: "hidden",
	},
	bar: {
		height: 6,
		backgroundColor: "#34C759",
		borderRadius: 3,
	},
	separator: {
		height: 10,
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
