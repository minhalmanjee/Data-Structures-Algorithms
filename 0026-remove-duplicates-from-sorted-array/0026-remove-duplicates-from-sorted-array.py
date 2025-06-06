class Solution(object):
    def removeDuplicates(self, nums):
        newarr = []
        for num in nums:
            if num not in newarr:
                newarr.append(num)
                
        for i in range(len(newarr)):
            nums[i] = newarr[i]

        return len(newarr)

        