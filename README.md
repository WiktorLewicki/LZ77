# **LZ77 Lossless Compression Algorithm Implementation**

Demonstration: [https://youtu.be/MVvS0YqbE_Q](https://youtu.be/MVvS0YqbE_Q)

This project presents an implementation of the **LZ77 lossless compression algorithm**.

**Key Features:**

- **Time Complexity:** The algorithm operates with a time complexity of **O(n × (log₂(n))²)**.

- **Karp-Miller-Rosenberg (KMR) Algorithm:** Incorporates the Karp-Miller-Rosenberg algorithm to enhance the efficiency of pattern matching during compression.

- **Multithreading Support:** Utilizes multithreading to accelerate the compression process, leveraging multiple CPU cores for improved performance.

**Platform-Specific Instructions:**

- **Linux:**
  1. Install the necessary packages listed in `packages.txt`.
  2. Navigate to the project directory and execute:
     ```bash
     bash build.sh
     ```

- **Windows:**
  1. Navigate to the `WIN` folder.
  2. Run the executable `LZ77.exe`.

**Usage:**

To compress a file, select the desired input file and specify the output location and filename. The application will process the file and generate a compressed version using the LZ77 algorithm.





The sample file used in the demonstration is sourced from [https://sjp.pl/sl/growy/](https://sjp.pl/sl/growy/).
In the demonstration, the file compression took approximately 23 seconds. On a significantly less powerful laptop, the same compression process took up to 95 seconds.
