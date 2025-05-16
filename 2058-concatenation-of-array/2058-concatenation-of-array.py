class Solution(object):
    def getConcatenation(self, nums):
        result = [0] * 2 * len(nums)
        for i in range(len(nums)):
            result[i] = nums[i]
            result[len(nums) + i] = nums[i]
        return result
