class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        l = 0
        r = 1
        mp = 0
        p = 0
        while r < len(prices):
            if prices[r] > prices[l]:
                p = prices[r] - prices[l]
                mp = max(mp,p)
            else:
                l = r
            r += 1
        return mp

                

        