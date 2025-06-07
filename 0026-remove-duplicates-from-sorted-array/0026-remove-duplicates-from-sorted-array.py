class Solution(object):
    def removeDuplicates(self, nums):
        lp = 0
        for rp in range(1, len(nums)):
            if nums[rp] != nums[lp]:
                lp += 1
                nums[lp] = nums[rp]
        return lp+1         




'''        newarr = []
        for num in nums:
            if num not in newarr:
                newarr.append(num)
        for i in range(len(newarr)):
            nums[i] = newarr[i]

        return len(newarr)

        nums[:] = sorted(set(nums))
		return len(nums)
'''

        