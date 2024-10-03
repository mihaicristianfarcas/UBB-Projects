#
# Functions section
#
def create_products_dictionary():
    return {'product_name': [], 'quantity': [], 'item_price': []}

def add_a_product(products, product_name: str, quantity, item_price):
    try:
        quantity = int(quantity)
        item_price = int(item_price)
        if quantity >= 0 or item_price >= 0:
            products['product_name'].append(product_name)
            products['quantity'].add(quantity)
            products['item_price'].append(item_price)
    except ValueError:
        print("Invalid input values!")

def remove_product(products, product_name: str):
    if product_name not in products['product_name']:
        print("Invalid name!")
    products.pop(product_name)


def list_all(products):
    print(products)


def list_total(products):
    sum_of_values = 0
    for i in products:
        sum_of_values += products[i]['quantity'] * products[i]['item_price']
    return sum_of_values

#
# User interface section
#
def user_command_processing():
    user_command = input()
    user_command_split = user_command.split(sep=' ')
    products = create_products_dictionary()
    try:
        assert user_command_split[0] in ["add", "remove", "list"]

        if user_command_split[0] == "add":
            if len(user_command_split) < 3:
                print("Error: Not enough input parameters!")
            else:
                add_a_product(products, user_command_split[1], user_command_split[2], user_command_split[3])

        if user_command_split[0] == "remove":
            remove_product(products, user_command_split[1])

        if len(user_command_split) == 2:
            if user_command_split[0] == "list":
                if user_command_split[1] == "all":
                    list_all(products)
                elif user_command_split[1] == "total":
                    list_total(products)
                else:
                    print("Invalid command!")
    except AssertionError:
        print("Invalid command!")


def main_menu():
    while True:
        user_command_processing()


main_menu()
