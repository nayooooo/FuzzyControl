import numpy as np
import matplotlib.pyplot as plt


plt.rcParams['font.sans-serif'] = 'SimHei'
plt.rcParams['axes.unicode_minus'] = False


if __name__ == '__main__':
    score = np.linspace(0, 100, 100)
    level = {}
    level['优秀'] = np.concatenate((np.zeros(90), np.linspace(0, 1, 10)))
    level['良好'] = np.concatenate((np.zeros(70), np.linspace(0, 1, 15), np.linspace(1, 0, 15)))
    level['及格'] = np.concatenate((np.zeros(60), np.linspace(0, 1, 15), np.linspace(1, 0, 15), np.zeros(10)))
    level['不及格'] = np.concatenate((np.linspace(1, 0, 60), np.zeros(40)))
    plt.figure(figsize=(8, 6))
    plt.plot(score, level['优秀'], label='优秀')
    plt.plot(score, level['良好'], label='良好')
    plt.plot(score, level['及格'], label='及格')
    plt.plot(score, level['不及格'], label='不及格')
    plt.xlabel('分数')
    plt.ylabel('隶属度')
    plt.title('分数隶属度函数曲线')
    plt.legend()
    plt.grid(True)
    plt.xlim(0, 100)
    plt.ylim(0, 1)
    plt.show()
    print('Done!')
