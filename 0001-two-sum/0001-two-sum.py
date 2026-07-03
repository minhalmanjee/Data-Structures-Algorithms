class Solution(object):
    def twoSum(self, nums, target):
        newnums = sorted((num,i) for i, num in enumerate(nums))
        left, right = 0, len(newnums) - 1
        while left < right:
            cur_sum = newnums[left][0] + newnums[right][0]
            if cur_sum == target:
                return [newnums[left][1], newnums[right][1]]
            elif cur_sum < target:
                left += 1
            else:
                right -= 1


    

        


# class Solution(object):
#     def twoSum(self, nums, target):
#         for a in range(len(nums)-1):
#             for b in range(a+1, len(nums)):
#                 if nums[a] + nums[b] == target:
#                     return [a,b]

        