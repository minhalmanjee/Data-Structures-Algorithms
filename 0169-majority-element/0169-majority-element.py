class Solution(object):
    def majorityElement(self, nums):
        cmap = {}
        for num in nums:
            if num in cmap:
                cmap[num] += 1
            else:
                cmap[num] = 1

        return(max(cmap, key=cmap.get))


        