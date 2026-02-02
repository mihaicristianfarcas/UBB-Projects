import React, { useState, useEffect } from "react";
import {
	View,
	StyleSheet,
	TextInput,
	TouchableOpacity,
	Alert,
	ActivityIndicator,
	KeyboardAvoidingView,
	Platform,
	ScrollView,
} from "react-native";
import { useRouter } from "expo-router";
import NetInfo from "@react-native-community/netinfo";
import { ThemedText } from "@/components/themed-text";
import { createSale } from "@/utils/api";
import { log } from "@/utils/logger";

export default function AddSaleScreen() {
	const router = useRouter();
	const [isOnline, setIsOnline] = useState(false);
	const [submitting, setSubmitting] = useState(false);

	const [date, setDate] = useState("");
	const [amount, setAmount] = useState("");
	const [type, setType] = useState("");
	const [category, setCategory] = useState("");
	const [description, setDescription] = useState("");

	useEffect(() => {
		const unsubscribe = NetInfo.addEventListener((state) => {
			const online = state.isConnected ?? false;
			setIsOnline(online);
			log(`Network: ${online ? "Online" : "Offline"}`, "info");
		});
		NetInfo.fetch().then((state) => setIsOnline(state.isConnected ?? false));
		return () => unsubscribe();
	}, []);

	const handleSubmit = async () => {
		if (!isOnline) {
			Alert.alert("Offline", "Adding sales requires an internet connection");
			return;
		}

		const trimmedDate = date.trim();
		const amountNum = parseFloat(amount);
		const trimmedType = type.trim();
		const trimmedCategory = category.trim();
		const trimmedDescription = description.trim();

		if (!trimmedDate) {
			Alert.alert("Validation", "Date is required");
			return;
		}
		if (!/^\d{4}-\d{2}-\d{2}$/.test(trimmedDate)) {
			Alert.alert("Validation", "Date must be in YYYY-MM-DD format");
			return;
		}
		if (isNaN(amountNum)) {
			Alert.alert("Validation", "Amount must be a valid number");
			return;
		}
		if (!trimmedType) {
			Alert.alert("Validation", "Type is required");
			return;
		}
		if (!trimmedCategory) {
			Alert.alert("Validation", "Category is required");
			return;
		}
		if (!trimmedDescription) {
			Alert.alert("Validation", "Description is required");
			return;
		}

		setSubmitting(true);
		const response = await createSale({
			date: trimmedDate,
			amount: amountNum,
			type: trimmedType,
			category: trimmedCategory,
			description: trimmedDescription,
		});
		setSubmitting(false);

		if (response.error) {
			Alert.alert("Error", response.error);
			return;
		}

		if (response.data) {
			Alert.alert("Success", "Sale created successfully", [
				{ text: "OK", onPress: () => router.back() },
			]);
		}
	};

	return (
		<KeyboardAvoidingView
			style={styles.container}
			behavior={Platform.OS === "ios" ? "padding" : "height"}
		>
			<ScrollView contentContainerStyle={styles.scrollContent}>
				{!isOnline && (
					<View style={styles.offlineBanner}>
						<ThemedText style={styles.offlineText}>
							You are offline. Adding sales requires a connection.
						</ThemedText>
					</View>
				)}

				<View style={styles.card}>
					<ThemedText type="subtitle" style={styles.cardTitle}>
						New Sale
					</ThemedText>

					<ThemedText style={styles.label}>Date</ThemedText>
					<TextInput
						style={styles.input}
						placeholder="YYYY-MM-DD"
						placeholderTextColor="#999"
						value={date}
						onChangeText={setDate}
						autoCapitalize="none"
					/>

					<ThemedText style={styles.label}>Amount</ThemedText>
					<TextInput
						style={styles.input}
						placeholder="e.g. 250000"
						placeholderTextColor="#999"
						value={amount}
						onChangeText={setAmount}
						keyboardType="numeric"
					/>

					<ThemedText style={styles.label}>Type</ThemedText>
					<TextInput
						style={styles.input}
						placeholder="sale, lease, or rent"
						placeholderTextColor="#999"
						value={type}
						onChangeText={setType}
						autoCapitalize="none"
					/>

					<ThemedText style={styles.label}>Category</ThemedText>
					<TextInput
						style={styles.input}
						placeholder="house, condo, or land"
						placeholderTextColor="#999"
						value={category}
						onChangeText={setCategory}
						autoCapitalize="none"
					/>

					<ThemedText style={styles.label}>Description</ThemedText>
					<TextInput
						style={[styles.input, styles.textArea]}
						placeholder="Describe the property..."
						placeholderTextColor="#999"
						value={description}
						onChangeText={setDescription}
						autoCapitalize="sentences"
						multiline
						numberOfLines={3}
					/>

					<TouchableOpacity
						style={[
							styles.submitButton,
							(submitting || !isOnline) && styles.buttonDisabled,
						]}
						onPress={handleSubmit}
						disabled={submitting || !isOnline}
					>
						{submitting ? (
							<ActivityIndicator size="small" color="#fff" />
						) : (
							<ThemedText style={styles.submitButtonText}>
								Create Sale
							</ThemedText>
						)}
					</TouchableOpacity>
				</View>
			</ScrollView>
		</KeyboardAvoidingView>
	);
}

const styles = StyleSheet.create({
	container: {
		flex: 1,
		backgroundColor: "#F2F2F7",
	},
	scrollContent: {
		padding: 16,
		paddingBottom: 40,
	},
	offlineBanner: {
		backgroundColor: "#FFF3CD",
		borderRadius: 10,
		padding: 14,
		marginBottom: 16,
		borderLeftWidth: 4,
		borderLeftColor: "#FFC107",
	},
	offlineText: {
		fontSize: 14,
		color: "#856404",
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
	cardTitle: {
		marginBottom: 20,
		color: "#1C1C1E",
	},
	label: {
		fontSize: 13,
		fontWeight: "600",
		color: "#8E8E93",
		marginBottom: 6,
		textTransform: "uppercase",
		letterSpacing: 0.5,
	},
	input: {
		borderWidth: 1,
		borderColor: "#E5E5EA",
		borderRadius: 10,
		padding: 14,
		fontSize: 16,
		marginBottom: 16,
		backgroundColor: "#F9F9FB",
		color: "#1C1C1E",
	},
	textArea: {
		height: 80,
		textAlignVertical: "top",
	},
	submitButton: {
		backgroundColor: "#34C759",
		paddingVertical: 16,
		borderRadius: 12,
		alignItems: "center",
		marginTop: 8,
	},
	buttonDisabled: {
		opacity: 0.5,
	},
	submitButtonText: {
		color: "#fff",
		fontSize: 17,
		fontWeight: "700",
	},
});
