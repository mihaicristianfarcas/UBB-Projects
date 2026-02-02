import { Tabs } from "expo-router";
import { Ionicons } from "@expo/vector-icons";

export default function TabLayout() {
	return (
		<Tabs
			screenOptions={{
				headerShown: true,
				headerStyle: { backgroundColor: "#F2F2F7" },
				headerTintColor: "#1C1C1E",
				headerShadowVisible: false,
				tabBarActiveTintColor: "#007AFF",
				tabBarInactiveTintColor: "#8E8E93",
				tabBarStyle: {
					backgroundColor: "#fff",
					borderTopColor: "#E5E5EA",
					paddingBottom: 4,
				},
			}}
		>
			<Tabs.Screen
				name="index"
				options={{
					title: "Sales",
					headerTitle: "Sales",
					tabBarIcon: ({ color, size }) => (
						<Ionicons name="home-outline" size={size} color={color} />
					),
				}}
			/>
			<Tabs.Screen
				name="reports"
				options={{
					title: "Reports",
					headerTitle: "Reports",
					tabBarIcon: ({ color, size }) => (
						<Ionicons name="bar-chart-outline" size={size} color={color} />
					),
				}}
			/>
			<Tabs.Screen
				name="insights"
				options={{
					title: "Insights",
					headerTitle: "Insights",
					tabBarIcon: ({ color, size }) => (
						<Ionicons name="bulb-outline" size={size} color={color} />
					),
				}}
			/>
		</Tabs>
	);
}
