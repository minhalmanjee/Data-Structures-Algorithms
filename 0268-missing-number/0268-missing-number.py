class Solution:
    def missingNumber(self, nums: List[int]) -> int:
        nums.sort()
        for i in range(len(nums)):
            if i == nums[i]:
                continue
            else:
                return i
        return len(nums)
        