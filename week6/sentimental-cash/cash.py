from cs50 import get_float

DOLLAR: int = 100
QUARTER: int = 25
DIME: int = 10
NICKEL: int = 5
PENNY: int = 1
available_coins = [QUARTER, DIME, NICKEL, PENNY]

while True:
    change: float = get_float("Change: ")
    if change >= 0:
        break

total_coins = 0
# convert the float to "cents" by multiplying by 100
cents = round(change * DOLLAR)
# start looping on the coins and figuring out how many go into the total cents
for current_coin in available_coins:
    # use floor division to get whole number of coins
    coins_in_cents = cents // current_coin
    # add those to the total
    total_coins += coins_in_cents
    # use modulo to get remainder and mutate the cents
    cents = cents % current_coin

print(total_coins)
