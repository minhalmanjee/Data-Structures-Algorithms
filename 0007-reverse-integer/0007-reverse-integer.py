class Solution:
    def reverse(self, x: int) -> int:
        sign = -1 if x < 0 else 1
        tstr = str(abs(x))
        reverse = tstr[::-1]
        reverseint = int(reverse) * sign
        return reverseint if -2**31 <= reverseint <= 2**31 - 1 else 0



        