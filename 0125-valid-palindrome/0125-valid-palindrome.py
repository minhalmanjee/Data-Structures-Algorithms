class Solution:
    def isPalindrome(self, s: str) -> bool:
        n = len(s)
        L, R = 0, n-1
        while L < R:
            if not s[L].isalnum():
                L += 1
                continue
                
            elif not s[R].isalnum():
                R -= 1
                continue
                
            elif s[L].lower() != s[R].lower():
                return False
            R -= 1 
            L += 1
        return True   
        

            

        