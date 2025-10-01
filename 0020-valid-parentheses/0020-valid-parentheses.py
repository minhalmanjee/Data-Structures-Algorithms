class Solution:
    def isValid(self, s: str) -> bool:
        if len(s) % 2 == 1:
            return False
        stack = []
        pairs = {')': '(', ']': '[', '}': '{'}

        for ch in s:
            if ch in ('(', '[', '{'):
                stack.append(ch)
            else:
                if not stack or stack[-1] != pairs.get(ch):
                    return False
                stack.pop()
        return not stack

            
        