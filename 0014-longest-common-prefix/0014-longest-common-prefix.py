
class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        if not strs:
            return ""
        
        strs.sort()
        str1 = strs[0]
        str2 = strs[-1]
        
        i = 0
        retstr = ""
        while i < len(str1) and i < len(str2):
            if str1[i] == str2[i]:
                retstr += str1[i]
                i += 1
            else:
                break
        
        return retstr

        
        