import sys
import re

#Philip Chang - CS  325- Dynamic Programming Change

# https://www.youtube.com/watch?annotation_id=annotation_2195265949&feature=iv&src_vid=Y0ZqKpToTic&v=NJuKJ8sasGk for help
# demon = demoniations of coins
# change = amount you need
# tried to use naming scheme in Project but get lost thus I have weird variables names that make sense to me
def changedp(demon, change):
    #checks in case there are no denominations or no change
    if demon == None or change == 0:
        return final, 0
#creating arrays to hold the min amount of coins, the current coin being used last, and the count for each denomination
    min_coins = [0] #min is 0 coins intialized
    current_coins = [0] #current set to the last used intialized to 0
    final = [0 for i in demon] #using a counter so initalizing all values to zero for each denomination count


    #for loop that creates sub problems to calculate the change needed for each possible change to the total change we want
    for g2g in range(1, change + 1): #start at 1 cause zero change is zero already and taken care of in base case, +1 cause we use 1 coin definetly
        c_total = g2g #current total set to current iteration for amount of change
        current_coin = demon[0] #setting the last coin to be used to be the lowest denomination

        #nested for loop that uses the g2g or the iteration value to calculate how many of each denomination we need
        for x in [denomination for denomination in demon if denomination <= g2g]: #for lopo goes through denominations in the demoninations of coins only if the denomination is less  than g2g
            if min_coins[g2g - x] + 1 < c_total:
                c_total = min_coins[g2g - x] + 1
                current_coin = x
        #add these values to the  min coins and current coin
        min_coins.append(c_total)
        current_coins.append(current_coin)

    #time to caculate the actual minimum coins from the value
    bank = change
    while bank > 0: #while the amount in the bank is bigger than zero start subtracting denominations
        money = current_coins[bank]#money you take away from bank is set to the last used coin at current position of change
        final[demon.index(money)] += 1 #the latest used coin counter is increased by 1
        bank = bank - money #taking money from the bank

    final_coins = min_coins[change] #the amount of coins used for the specific change from appending every small sub problem's min coins
    #returning the amount of each denomination, and the total amount of coins
    return final, final_coins




def mo_money():
    filename = sys.argv[1]
    inputfile = open(filename+'.txt')
    outputfile = open(filename+'change.txt', 'a')
    outputfile.write('Algorithm #3 Dynamic Programming changedp: \n')
    for line in inputfile:
        demon = list(map(int, line.split(' ')))
        bank = int(next(inputfile))
        (coins, total) = changedp(demon, bank)
        outputfile.write(str(demon).replace('[','').replace(']', '') + '\n')
        outputfile.write(str(coins).replace('[','').replace(']', '') + '\n')
        outputfile.write(str(total).replace('[','').replace(']', '') + '\n\n')

    inputfile.close()
    outputfile.close()

