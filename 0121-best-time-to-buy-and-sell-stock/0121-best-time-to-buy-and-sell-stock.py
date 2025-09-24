class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        if len(prices) < 2:
            return 0

        buy = prices[0]
        sell = 0
        profit = 0
        for r in range(1, len(prices)):
            if prices[r] < buy:
                buy = prices[r]
            sell = prices[r]
            profit = max(profit, sell - buy)
        return profit


        