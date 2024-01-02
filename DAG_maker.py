import random as rand
import sys


def main():
    for file in range(int(sys.argv[1])):
        edges = set()
        number_vertices = rand.randint(10, int(sys.argv[2]))
        vertex_array = rand.sample(list(range(number_vertices)), number_vertices)

        for vertex_idx in range(number_vertices):
            out_degree = rand.randint(0, number_vertices - vertex_idx - 1)
            for _ in range(out_degree):
                outgoing_idx = rand.randint(vertex_idx + 1, number_vertices - 1)
                edges.add((vertex_array[vertex_idx], vertex_array[outgoing_idx]))

        with open(f"GRAPHS/DAG_{number_vertices}_{file}.txt", "w") as output:
            output.write(f"1\n0\n{number_vertices}\n{len(edges)}\n")
            for start, end in edges:
                output.write(f"{start} {end}\n")


if __name__ == "__main__":
    main()
