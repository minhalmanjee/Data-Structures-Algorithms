class Solution:
    def isIsomorphic(self, s: str, t: str) -> bool:
        if len(s) != len(t):
            return False
        
        mapping = {}
        mapped_values = set()

        for cs, ct in zip(s, t):
            if cs in mapping:
                if mapping[cs] != ct:
                    return False
            else:
                if ct in mapped_values:
                    return False
                mapping[cs] = ct
                mapped_values.add(ct)

        return True
