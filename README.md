# ColourS: Therapeutic Programs for Eye-Hand Coordination

## Overview
*ColourS* is a spectrum of therapeutic programs designed to assess and train eye hand coordination.

Eye hand coordination is the effortful concatenation of eye movement with hand movement. It allows visual input to guide manual tasks (e.g. grasping, reaching) and manual proprioception to guide visual tasks (e.g. gaze control, focus). 

Poor eye hand coordination accompanies many syndromes and conditions such as cerebral palsy, autism spectrum disorders, decreased muscle tone, and visual disorders like optic ataxia.

Deficient eye hand coordination is often the sum of neuro-physiological disturbances and can be treated by strengthening the oculo-manual muscles. Prolonged therapy may also reinforce the neural pathways that sequence visual input with hand movement, opening a number of downstream motor capacities to improvement.

## Features

- **Assessment Tools**: Evaluate the current state of eye-hand coordination.
- **Training Modules**: Exercises to improve synchronization between visual input and hand movements.
- **Multi-Condition Support**: Tailored for various conditions affecting motor skills.

## Project Structure

```
colours/
├── blue/               # Blue spectrum training programs
├── green/              # Green spectrum training programs
├── include/            # Header files and shared resources
├── red/                # Red spectrum training programs
├── sounds/             # Auditory feedback resources
├── violet/             # Violet spectrum training programs
├── colours.c           # Core implementation of ColourS
├── CMakeLists.txt      # Build configuration
├── LICENSE             # License information
└── README.md           # Project documentation
```

## Installation

To set up ColourS on your system:

1. **Clone the Repository**:
   ```sh
   git clone https://github.com/wi11iamk/colours.git
   cd colours
   ```

2. **Build the Project**:
   Ensure you have [CMake](https://cmake.org/) installed. Then, run:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

## Running the Programs

After building, you can execute the therapeutic programs:

```sh
./colours
```

Follow the on-screen instructions to select and run specific training modules.

## Contributing

Contributions are welcome! If you have suggestions or improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [GPL-3.0 License](LICENSE).

## Author

**William D. Kistler** ([@wi11iamk](https://github.com/wi11iamk))
