# use this for hot map magic to sum columns
from operator import sub, add

"""
This is an array that holds the maximum number of useable resources per resource. Resources could be
hardware devices or cores or other usable computer resources. The count dictates the total number of
those resources available. 
"""
resources = [5, 2, 4, 3]

"""
This is an array that hold the number of used resources (on columns) per process using them (on a row) 
"""
allocation = [
    [2, 0, 1, 1],
    [0, 1, 0, 0],
    [1, 0, 1, 1],
    [1, 1, 0, 1]
]


def get_allocated() -> []:
    """
    get allocated resources currently by summing the columns of the allocation matrix.
    This gives us a clear idea of what the system is "using" at the moment, which will help
    us make sure that any additional requests for resources wont result in deadlock.

    :return: an array of the columns summed, representing what resources are currently allocated
    """
    allocated = [0] * len(allocation)
    for i in range(0, len(allocation[0])):
        col = []
        for j in range(0, len(allocation)):
            col.append(allocation[j][i])
        allocated[i] += sum(col)

    return allocated


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


def get_available() -> []:
    """
    TODO Oh man this is a doozy the example is wrong on this one but it looks to be
    total - allocated is this number, which would tell us how many we have available left.
    Then, we can use that available vector to understand which process can run/whether a process
    would created deadlock.
    """
    return list(map(sub, resources, get_allocated()))


def detect_deadlock():
    """
    We need to check to see if there is a row in our "resources needed" values that is allowed to be
    allocated safely. If a row in the needs matrix is less than our available vector, then we can
    safely proceed with allocating that resource. Otherwise, if there are no possible vectors in the
    needs matrix that satisfy that, then we have "overdrawn our account" and we are not in a safe state.
    Deadlock is possible!

    :return: returns (INT) index of the resource that works or False if none are present
    """
    isOverdrawn = True

    for index, need in enumerate(needed):
        canRunProcess = True
        for i in range(len(need)):
            if get_available()[i] < need[i]:
                canRunProcess = False

        if canRunProcess:
            print(f"Resource {need} can be safely allocated")
            isOverdrawn = False
            return index

    return isOverdrawn


if __name__ == "__main__":
    #
    # Deadlock detection
    #
    # initial check for deadlock, check and see if anything in the "need" array has resources that are allowed
    # to be met

    while len(needed) > 0:
        result = detect_deadlock()
        if result is not False:
            process_row = needed.pop(result)
            allocated_for_process = allocation.pop(result)
        else:
            print("Deadlock Detected")

    # if we're good, loop through the safe options and

    """
        for i in range(len(needed[0])):
        col = []
        for j in range(0, len(needed)):
            col.append(needed[j][i])
        if (sum(col)) > 0:
            print(col)
            print(sum(col))
            print("~~~~")

    """
