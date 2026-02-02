import { API_URL } from "../config";
import { Transaction } from "../types/transaction";
import { log } from "./logger";

interface ApiResponse<T> {
	data?: T;
	error?: string;
}

async function request<T>(
	endpoint: string,
	options?: RequestInit,
): Promise<ApiResponse<T>> {
	try {
		log(`API ${options?.method || "GET"} ${endpoint}`, "info");
		const response = await fetch(`${API_URL}${endpoint}`, {
			headers: { "Content-Type": "application/json" },
			...options,
		});
		if (!response.ok) {
			const err = await response.json();
			log(`API Error: ${err.error || response.statusText}`, "error");
			return { error: err.error || response.statusText };
		}
		const data = await response.json();
		log(`API Success: ${endpoint}`, "success");
		return { data };
	} catch (error) {
		const message = error instanceof Error ? error.message : "Network error";
		log(`API Error: ${message}`, "error");
		return { error: message };
	}
}

export const getSales = () => request<Transaction[]>("/sales");

export const getSaleById = (id: number) => request<Transaction>(`/sale/${id}`);

export const createSale = (sale: Omit<Transaction, "id">) =>
	request<Transaction>("/sale", {
		method: "POST",
		body: JSON.stringify(sale),
	});

export const deleteSale = (id: number) =>
	request<{ success: boolean }>(`/sale/${id}`, { method: "DELETE" });

export const getAllSales = () => request<Transaction[]>("/allSales");
