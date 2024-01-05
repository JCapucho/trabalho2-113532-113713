num_graphs = 5 


for file in range(10, num_graphs + 10):
    number_vertices = pow(2, file)
    output = open(f"GRAPHS/SEQUENTIAL_DAG_{number_vertices}_2.txt", "w")
    output.write(f"1\n0\n{number_vertices + 1}\n{2 * number_vertices - 2}\n")
    for vertex_idx in range(number_vertices - 1):
        output.write(f"{vertex_idx} {vertex_idx + 1}\n")
        output.write(f"{vertex_idx} {vertex_idx + 2}\n")
    output.write(f"{number_vertices - 1} {number_vertices}\n")
        

    output.close()
