class Solution:
    def majorityElement(self, nums: List[int]) -> int:
        msize = (len(nums))/2
        pmap = {}
        for n in nums:
            pmap[n] = 1 + pmap.get(n, 0)
        return max(pmap, key=pmap.get)
        
        