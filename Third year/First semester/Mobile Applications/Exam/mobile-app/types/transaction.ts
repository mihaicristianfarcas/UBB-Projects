// On the server side, at least the following details are maintained:
// `id`: The unique identifier for the transaction. Integer value greater than zero.
// `date`: The date when the transaction occurred. A string in the format "YYYY-MM-DD".
// `amount`: The sale price or rent amount. A decimal value.
// `type`: The type of transaction (e.g., sale, lease, rent). A string of characters.
// `category`: The category of the property (e.g., house, condo, land). A string of characters.
// `description`: A description of the property. A string of characters.

export interface Transaction {
	id: number;
	date: string;
	amount: number;
	type: string;
	category: string;
	description: string;
}
