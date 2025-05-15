class Solution(object):
    def containsDuplicate(self, nums):
        numset = set()
        for i in nums:
            if i in numset:
                return True
            else:
                numset.add(i)
        return False