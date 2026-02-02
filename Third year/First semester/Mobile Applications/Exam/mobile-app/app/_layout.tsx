import { Stack } from "expo-router";

export default function RootLayout() {
	return (
		<Stack
			screenOptions={{
				headerStyle: { backgroundColor: "#F2F2F7" },
				headerTintColor: "#1C1C1E",
				headerShadowVisible: false,
				contentStyle: { backgroundColor: "#F2F2F7" },
			}}
		>
			<Stack.Screen name="(tabs)" options={{ headerShown: false }} />
			<Stack.Screen
				name="sale-detail"
				options={{ headerTitle: "Sale Details" }}
			/>
			<Stack.Screen
				name="add-sale"
				options={{ headerTitle: "Add Sale", presentation: "modal" }}
			/>
		</Stack>
	);
}
