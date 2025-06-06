class Solution(object):
    def majorityElement(self, nums):
        majority = 0
        vote = 0
        for num in nums:
            if vote == 0:
                vote += 1
                majority = num
            elif majority == num:
                vote +=1
            else:
                vote -=1
        return majority


''' cmap = {}
        for num in nums:
            if num in cmap:
                cmap[num] += 1
            else:
                cmap[num] = 1

        return(max(cmap, key=cmap.get)) '''





       


        