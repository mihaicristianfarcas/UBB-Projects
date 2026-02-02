import AsyncStorage from "@react-native-async-storage/async-storage";
import { Transaction } from "../types/transaction";
import { log } from "./logger";

const KEYS = {
	SALES: "@sales:list",
	ALL_SALES: "@sales:all",
	DETAIL_PREFIX: "@sales:detail:",
};

export const saveSales = async (sales: Transaction[]) => {
	try {
		await AsyncStorage.setItem(KEYS.SALES, JSON.stringify(sales));
		log("Saved sales to storage", "success");
	} catch (error) {
		log("Failed to save sales", "error");
	}
};

export const getLocalSales = async (): Promise<Transaction[]> => {
	try {
		const data = await AsyncStorage.getItem(KEYS.SALES);
		return data ? JSON.parse(data) : [];
	} catch (error) {
		log("Failed to get local sales", "error");
		return [];
	}
};

export const saveAllSales = async (sales: Transaction[]) => {
	try {
		await AsyncStorage.setItem(KEYS.ALL_SALES, JSON.stringify(sales));
		log("Saved all sales to storage", "success");
	} catch (error) {
		log("Failed to save all sales", "error");
	}
};

export const getLocalAllSales = async (): Promise<Transaction[]> => {
	try {
		const data = await AsyncStorage.getItem(KEYS.ALL_SALES);
		return data ? JSON.parse(data) : [];
	} catch (error) {
		log("Failed to get local all sales", "error");
		return [];
	}
};

export const saveSaleDetail = async (sale: Transaction) => {
	try {
		await AsyncStorage.setItem(
			KEYS.DETAIL_PREFIX + sale.id,
			JSON.stringify(sale),
		);
		log(`Saved sale detail ${sale.id} to storage`, "success");
	} catch (error) {
		log("Failed to save sale detail", "error");
	}
};

export const getLocalSaleDetail = async (
	id: number,
): Promise<Transaction | null> => {
	try {
		const data = await AsyncStorage.getItem(KEYS.DETAIL_PREFIX + id);
		return data ? JSON.parse(data) : null;
	} catch (error) {
		log("Failed to get local sale detail", "error");
		return null;
	}
};
