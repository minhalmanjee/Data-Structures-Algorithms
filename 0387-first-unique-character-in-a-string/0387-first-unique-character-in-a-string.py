class Solution:
    def firstUniqChar(self, s: str) -> int:
        seen = {}
        for ch in s:
            seen[ch] = seen.get(ch, 0) + 1
        
        for i, ch in enumerate(s):
            if seen[ch] == 1:
                return i
        
        return -1