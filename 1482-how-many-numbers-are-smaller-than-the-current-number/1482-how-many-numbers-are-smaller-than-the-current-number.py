class Solution:
    def smallerNumbersThanCurrent(self, nums: List[int]) -> List[int]:
        sorted_nums = sorted(nums)
        hmap = {}
        for i in range(len(sorted_nums)):
            if sorted_nums[i] not in hmap:
                hmap[sorted_nums[i]] = i
        result = []
        for num in nums:
            result.append(hmap[num])
        return result

        #explain: we sorted first & then stored index of sorted array and returned in arr the index of sorted num.
            



        