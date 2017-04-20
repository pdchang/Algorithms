#Philip Chang - CS 325 - Project 01 part 1
def enumeration(array):
    if len(array) == 0:
        return 0
    else:
        max_sum = 0

    for i in range(len(array)):
        for j in range(i, len(array)):
            temp = 0
            for k in range(i, j+1):
                temp += array[k]
            if temp > max_sum:
                max_sum = temp
                mindx = i
                maxdx = j
    return (array[mindx:(maxdx + 1)], max_sum)
