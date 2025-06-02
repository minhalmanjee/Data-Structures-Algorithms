class Solution(object):
    def twoSum(self, nums, target):
        for l in range(len(nums)):
            for r in range(l + 1, len(nums)):  # Ensure r starts after l
                temp = nums[l] + nums[r]
                if temp == target:
                    return [l, r]
        return []  # Return an empty list if no solution is found

                
        