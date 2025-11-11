class Solution:
    def numJewelsInStones(self, jewels: str, stones: str) -> int:
        js = set(jewels)
        i=0
        for char in stones:
            if char in js:
                i += 1
        return i
        