class Solution:
    def isPalindrome(self, x: int) -> bool:
        if x == 0:
            return True

        digits = str(x)
        i = len(digits)-1
        for d in range(len(digits)):
            if digits[d] == digits[i]:
                i -= 1
            else:
                return False
        return True
        