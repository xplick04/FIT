import sys

n = sys.argv[1]
n = int(n)

outputHeader = "p cnf " + str(n * n)    #for later...

outputStr = ''
cntCNF = 0

for i in range(0,n): #rows
    for j in range(1,n+1): #columns
        num = str(i*n+j)
        outputStr = outputStr + num + ' '
    outputStr = outputStr + ' 0' + '\n'
    cntCNF+=1

#rows
for i in range(0,n): #rows
    for j in range(1,n+1): #columns
        for k in range(j,n):
            num = str(-1*(i*n+j))
            num2 = str(-1*(i*n+k+1))
            outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
            cntCNF += 1


# #columns
for i in range(0,n): #rows
    for j in range(1,n+1): #columns
        for k in range(1,n-i):
            num = str(-1*(i*n+j))
            num2 = str(-1*(i*n+j+n*k))
            outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
            cntCNF += 1

# #topleft-bottomright diagonals
for i in range(0,n): #rows
    for j in range(1,n+1): #columns
        if(i >= j-1): #lower part
            for k in range(1,n-i):
                num = str(-1*(i*n+j))
                num2 = str(-1*(i*n+j+(n+1)*k))
                outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
                cntCNF += 1
        if(i < j-1): #upper part
            for k in range(1,n-j+1):
                num = str(-1*(i*n+j))
                num2 = str(-1*(i*n+j+(n+1)*k))
                outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
                cntCNF += 1

#topright-bottomleft diagonals lower
for i in range(0,n): #rows
    for j in range(n-i,n): #columns
            for k in range(1,n-i):
                num = str(-1*(i*n+j+1))
                num2 = str(-1*(i*n+j+1+(n-1)*k))
                outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
                cntCNF += 1

# #topright-bottomleft diagonals upper
for i in range(0,n): #rows
    for j in range(0,n-i): #columns
            for k in range(1,j+1):
                num = str(-1*(i*n+j+1))
                num2 = str(-1*(i*n+j+1+(n-1)*k))
                outputStr = outputStr + num + ' ' + num2 + ' 0' + '\n'
                cntCNF+=1

outputHeader = outputHeader + ' ' + str(cntCNF) + '\n'
final = outputHeader + outputStr
print(final)





