class Solution:
    def isSubsequence(self, s: str, t: str) -> bool:
        s1 = 0
        for t1 in t:
            if len(s) == 0:
                return True
            if s[s1] == t1:
                s1 += 1
            if s1 >= len(s):
                break
                return False
        if s1 == len(s):
            return True
        else:
            return False