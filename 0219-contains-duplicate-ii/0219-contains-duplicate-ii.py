class Solution(object):
    def containsNearbyDuplicate(self, nums, k):
        dic = {}
        for i, num in enumerate(nums):
            if num in dic and i-dic[num] <= k:
                return True
            dic[num] = i
        return False

       
'''
        n = set()
        l = 0
        for r in range(len(nums)):
            if nums[r] in n:
                return True
            n.add(nums[r])
            if r-l >= k:
                n.remove(nums[l])
                l += 1
        return False 
        
'''