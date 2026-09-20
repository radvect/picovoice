from typing import Sequence
import random
import numpy as np

a = [random.random() for _ in range(365)]


def prob_rain_more_than_n(p: Sequence[float], n: int) -> float:
    
    dp = np.zeros((366, 366))
    dp[0,0] = 1
    for i in range(1, dp.shape[1]):
        print(i)
        dp[0][i] = dp[0][i - 1] * (1 - p[i - 1])
    dp[1][1] = p[0]


    ##################################
    for i in range(1, dp.shape[0]):
        for j in range(2,dp.shape[1]):
            dp[i][j] = (
                dp[i][j - 1] * (1 - p[j - 1])
                + dp[i - 1][j - 1] * p[j - 1])



    return np.sum(dp[n:, 365])

