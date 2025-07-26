class Solution:
    def isIsomorphic(self, s: str, t: str) -> bool:
        if len(s) != len(t):
            return False

        ds = {}
        dt = {}
        for cs, ct in zip(s,t):
            if(cs in ds and ds[cs] != ct) or (ct in dt and dt[ct] != cs):
                return False
            ds[cs] = ct
            dt[ct] = cs
        return True

        