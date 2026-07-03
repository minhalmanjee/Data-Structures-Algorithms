class Solution(object):
    def twoSum(self, nums, target):
        for a in range(len(nums)-1):
            for b in range(a+1, len(nums)):
                if nums[a] + nums[b] == target:
                    return [a,b]

        