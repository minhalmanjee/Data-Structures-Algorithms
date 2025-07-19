class Solution(object):
    def maxProfit(self, prices):
        buy = prices[0]
        profit = 0
        for i in range(1, len(prices)):
            if buy > prices[i]:
                buy = prices[i]
            else:
                currentprofit = prices[i] - buy
                if profit < currentprofit:
                    profit = currentprofit
        return profit