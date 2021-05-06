# use this for hot map magic to sum columns
from operator import sub, add

import random


def get_allocated() -> []:
    """
    get allocated resources currently by summing the columns of the allocation matrix.
    This gives us a clear idea of what the system is "using" at the moment, which will help
    us make sure that any additional requests for resources won't result in deadlock.

    :return: an array of the columns summed, representing what resources are currently allocated
    """
    allocated = [0] * len(allocation)
    for i in range(0, len(allocation[0])):
        col = []
        for j in range(0, len(allocation)):
            col.append(allocation[j][i])
        allocated[i] += sum(col)

    return allocated


def get_available() -> []:
    """
    Oh man this is a doozy the example is wrong on this one but it looks to be
    total - allocated is this number, which would tell us how many we have available left.
    Then, we can use that available vector to understand which process can run/whether a process
    would created deadlock.
    """
    return [(0 if x < 0 else x) for x in list(map(sub, resources, get_allocated()))]


def detect_deadlock():
    """
    We need to check to see if there is a row in our "resources needed" values that is allowed to be
    allocated safely. If a row in the needs matrix is less than our available vector, then we can
    safely proceed with allocating that resource. Otherwise, if there are no possible vectors in the
    needs matrix that satisfy that, then we have "overdrawn our account" and we are not in a safe state.
    Deadlock is possible!

    :return: returns (INT) index of the resource that works or False if none that are suitable are present
    """

    for index in range(len(needed)):
        # is this need less than our available resources? (safe to run)
        canRun = []
        for a, n in zip(get_available(), needed[index]):
            if a < n:
                canRun.append(False)

        if len(canRun) == 0:
            print(f"Resource {needed[index]} at index {index} can be safely allocated")
            return index

    return False


if __name__ == "__main__":

    # use default allocation array?
    r = input("Run alternate? (Y/n)")
    if r == "Y" or r == "y":
        generate = True
    else:
        generate = False

    """
    Generate process info
    
    Since we know we need to generate a couple of different things, we may as well list them:
    1. Resources - pretend this is what we're divying up, and should be decided by the user
    2. Allocated - this should be a portion of what we're allowed, summed on the columns. If we
        allocate too much here, then it's awkward cause then we're in an unsafe state and we 
        can get deadlocked. But ideally this matrix sums along the columns to be less than our
        resources matrix plus a little wiggle room so that we can wrap some processes up.
    3. Needed - Speaking of wrapping processes up, our final little bit of wiggle room needs to fit
        whatever these values are 
        
        
    
    This is an array that holds the maximum number of useable resources per resource. Resources could be
    hardware devices or cores or other usable computer resources. The count dictates the total number of
    those resources available. 
    """
    resources = [5, 2, 4, 3]
    r_n = len(resources)

    num_tasks = 10

    if generate:
        """
        This sequence isn't too sophisticated, it generates what it thinks will be a resource set
        and allocated/needed array to make something that could either end in deadlock due to over-allocation
        or could end with all processes being 
        """
        resources *= 3
        allocation = []
        needed = []
        ceiling_resource = sum(resources) / len(resources)

        for i in range(num_tasks):
            row = []
            for j in range(r_n):
                row.append(random.randint(0, int(ceiling_resource * 0.3)))

            allocation.append(row)

        for i in range(num_tasks):
            row = []
            for j in range(r_n):
                row.append(random.randint(0, int(ceiling_resource * 0.3)))

            needed.append(row)
    else:
        """
        This is the default sequence! This one is guaranteed to finish without deadlock.
        
        This is an array that hold the number of used resources (on columns) per process using them (on a row) 
        """
        allocation = [
            [2, 0, 1, 1],
            [0, 1, 0, 0],
            [1, 0, 1, 1],
            [1, 1, 0, 1]
        ]

        """
        This is an array that holds the "needed" processes to complete the work. For example, a process with
        requirements [0, 0, 1, 0] needs 0 of every resource type except the 2nd (0-indexed) one which has a 
        requirement of 1. This means that if this process is to run 'safely' according to Bankers algorithm,
        then the 'available' resource vector had at least better read [0,0,1,0] to accommodate for that additional
        resource, otherwise a deadlock is possible due to overallocation. 
        """
        needed = [
            [1, 1, 0, 0],
            [0, 1, 1, 2],
            [3, 1, 0, 0],
            [0, 0, 1, 0]
        ]

    #
    # Deadlock detection
    #
    # initial check for deadlock, check and see if anything in the "need" array has resources that are allowed
    # to be met

    is_locked = False
    command = None

    while len(needed) > 0 and not is_locked:
        result = detect_deadlock()
        if result is not False:
            try:
                process_row = needed.pop(result)
                allocation[result] = list(map(add, allocation[result], process_row))
                available = get_available()

                print(f"Removing resources needed by {result} process using {allocation[result]}")
                print("~~~~~~~")
                allocation[result] = [0 for x in allocation[result]]
            except IndexError as e:
                print(e)
        else:
            is_locked = True
            print(f"Process failed to allocate")

    if not is_locked:
        print("Processes have been completed successfully")
    else:
        print("Deadlock Detected")
