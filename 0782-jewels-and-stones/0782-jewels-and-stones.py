class Solution:
    def numJewelsInStones(self, jewels: str, stones: str) -> int:
        hmaps={}
        i=0
        for char in stones:
            if char in jewels:
                i += 1
        return i
        