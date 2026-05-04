import pandas as pd
import matplotlib.pyplot as plt

try:
    df = pd.read_csv('complexity.csv')
except FileNotFoundError:
    print("File complexity.csv not found!")
    exit()

plt.figure(figsize=(10, 6))


plt.plot(df['Digits'], df['E_Operations'], 'b-', linewidth=2, label='Exponenta (e)')
plt.plot(df['Digits'], df['PI_Operations'], 'r-', linewidth=2, label='Pi (Ramanujan)')

plt.title('Comparison of Computational Complexity of Algorithms')
plt.xlabel('Precision (significant figures)')
plt.ylabel('Number of Operations')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

plt.savefig('complexity_comparison.png', dpi=300)
print("Graph saved to file complexity_comparison.png")


plt.show()