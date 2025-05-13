# MetaWiper

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![CMake](https://img.shields.io/badge/CMake-3.15+-blue)](https://cmake.org/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/std/the-standard)
[![Qt](https://img.shields.io/badge/Qt-6.9.0+-green.svg)](https://www.qt.io/)

MetaWiper is a privacy-focused C++ project that provides:
- A modern desktop application for viewing and cleaning metadata from files
- A modular C++ library for metadata processing, designed for integration into other applications
- A command-line interface (CLI) for batch processing and automation (coming soon)

> [!WARNING]
> This project is in a very early stage of development. There are numerous unfinished features and potential instabilities. You may encounter various issues during use. Your understanding is appreciated, and feedback is welcome.

## Table of Contents
- [About](#about)
  - [What is Metadata?](#what-is-metadata)
  - [Who Should Use MetaWiper?](#who-should-use-metawiper)
  - [Features](#features)
  - [Supported File Types](#supported-file-types)
- [Prerequisites](#prerequisites)
  - [Dependencies Installation](#dependencies-installation)
  - [Other Platforms](#other-platforms)
- [Building from Source](#building-from-source)
  - [Clone the Repository](#clone-the-repository)
  - [Build with CMake (Windows)](#build-with-cmake-windows)
  - [Build Options](#build-options)
- [Usage Guide](#usage-guide)
- [Technical Architecture](#technical-architecture)
  - [Core Library](#core-library-meta_wiper_core)
  - [GUI Application](#gui-application-meta_wiper_gui)
  - [Command-Line Interface](#command-line-interface-planned)
  - [Data Flow](#data-flow)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Packaging](#packaging)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## About

MetaWiper is an open-source solution for viewing, editing, and removing metadata from various file types. It consists of a user-friendly desktop application, a reusable C++ core library, and (in future releases) a command-line interface for automation and scripting. This modular approach gives individuals, professionals, and organizations full control over their digital information by eliminating potentially sensitive metadata embedded in documents, images, and PDF files before sharing them.

> [!NOTE]
> MetaWiper is currently developed and tested primarily on Windows. Support for other platforms is planned but not fully tested.

### What is Metadata?

Metadata is "data about data" - information embedded within files that describes various aspects of the file itself. While metadata serves important purposes, it can also contain sensitive information you may not want to share:

- **In Photos**: GPS coordinates where the photo was taken, camera model, date/time, camera settings
- **In Documents**: Author name, organization, creation/modification dates, revision history, comments
- **In PDFs**: Author details, creation software, timestamps, hidden layers, embedded data

For example, sharing a photo might inadvertently reveal your home location via GPS data, or documents might expose confidential company information through hidden metadata. MetaWiper gives you control over what information remains in your files before sharing.

### Who Should Use MetaWiper?

- **Privacy-Conscious Individuals** who want to protect personal information when sharing photos and documents online
- **Professionals** handling sensitive client information in legal, healthcare, or financial sectors
- **Journalists and Activists** who need to protect sources and locations when sharing media
- **Business Users** preventing accidental exposure of confidential company information
- **Photographers** who want to selectively manage what camera and location data remains in their shared work

### Features

- 🔍 View all metadata contained within supported files
- 🧹 Clean specific metadata fields or remove all metadata at once
- 💾 Save cleaned versions with options for overwriting or creating new files
- 🎨 Modern, intuitive QML-based user interface
- 📊 Batch processing for multiple files

### Supported File Types

The following table shows the current support status for various operations across different file types:

| File Type | READ<br>(View Metadata) | CLEAN<br>(Remove Metadata) | EXPORT<br>(Save as Copy) | OVERWRITE<br>(Replace Original) |
|:--------:|:----------------------:|:------------------------:|:---------------------:|:-----------------------------:|
| **JPEG/JPG** | ✅ | ✅ | ✅ | ✅ |
| **PDF** | ✅ | 🚧 | 🚧 | 🚧 |
| **DOCX** | ✅ | 🚧 | 🚧 | 🚧 |

✅ Fully supported &nbsp;&nbsp; 🚧 In development

> [!NOTE]
> The operations listed (READ, CLEAN, EXPORT, OVERWRITE) generally correspond to the `operation_type` enum values (`READ`, `CLEAN`, `EXPORT`, `OVERWRITE`, `RESTORE`) defined in `core/include/base/file_handler.h`.

## Prerequisites

To build MetaWiper, you'll need the following dependencies:

- C++ compiler with C++17 support
- CMake 3.15 or higher
- Qt 6.9.0 or later recommended (Core, GUI, Widgets, Quick, QuickControls2)
- PoDoFo library 0.10.4 (PDF processing)
- Exiv2 library 0.28.5 (Image metadata processing)
- pugixml library 1.15 (XML processing for DOCX files)
- libzip library 1.11.3#1 (ZIP archive support)

### Dependencies Installation

We strongly recommend using [vcpkg](https://github.com/microsoft/vcpkg) package manager for handling dependencies on Windows, which simplifies the build process and ensures compatibility.

```powershell
# Install vcpkg if you don't have it
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat

# Install dependencies
vcpkg install podofo:x64-windows
vcpkg install exiv2:x64-windows
vcpkg install pugixml:x64-windows

# Install Qt using the Qt installer from https://www.qt.io/download
```

### Other Platforms

While MetaWiper is designed to be cross-platform, it has been primarily tested on Windows. The codebase should be compatible with other platforms, but additional configuration might be required.

## Building from Source

### Clone the Repository

```powershell
git clone https://github.com/Richard-Shaun/MetaWiper.git
cd meta_wiper
```

### Build with CMake (Windows)

```powershell
# Create a build directory
mkdir build
cd build

# Configure the project with vcpkg (recommended)
# Replace "C:/path/to/vcpkg" with your actual vcpkg installation path
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build all targets (or specify a target, e.g., meta_wiper_gui, meta_wiper_core)
cmake --build . --config Release
# Example for building a specific target:
# cmake --build . --config Release --target meta_wiper_gui

# Run
.\gui\Release\meta_wiper_gui.exe  # Windows
# On other platforms (Linux/macOS), the executable will typically be in a similar path, e.g., gui/meta_wiper_gui or build/gui/meta_wiper_gui
```

### Build Options

- `-DCMAKE_BUILD_TYPE=Release|Debug` - Set build type
- `-DCMAKE_PREFIX_PATH=/path/to/qt` - Specify Qt installation path if not detected automatically

## Usage Guide

1. **Launch the application** from the installed location or build directory
2. **Add files** by clicking the "Add" button or dragging files into the application
3. **View metadata** of selected files in the metadata panel
4. **Clean metadata** by selecting specific fields or using "Clean All" option
5. **Save cleaned files** with original name (overwrite) or as a new file

## Technical Architecture

MetaWiper is designed with a modular, extensible architecture that separates the core metadata processing engine from the user interface and (future) command-line interface. This enables maintainability, testability, and easy integration into other applications.

### Core Library (`meta_wiper_core`)

The core C++ library provides a unified API for metadata operations across multiple file types. Its architecture includes:

- **Abstraction Layer**: Exposes a single interface for all supported file operations, abstracting away format-specific details.
- **Processor Factory Pattern**: Dynamically instantiates the appropriate processor (PDF, JPEG, DOCX, etc.) based on file type.
- **Pluggable Processors**: Each file type is handled by a dedicated processor class, making it easy to add support for new formats.
- **Operation Framework**: Supports multiple operation types (`READ`, `CLEAN`, `OVERWRITE`, `EXPORT`, `RESTORE`), with consistent error handling and standardized `operation_result` structures (status, messages, metadata, warnings).
- **Batch Processing**: Provides methods for processing multiple files efficiently.
- **Thread-Safe Design**: Core functions are safe for concurrent use, supporting batch and UI-driven workflows.
- **Extensibility**: New file formats and operations can be added by implementing new processor classes and registering them with the factory.

```cpp
// Example of the core API usage
meta_wiper_core::meta_wiper_core_class processor;
auto result = processor.process_file(
    "document.pdf",
    file_handler::operation_type::READ
);
if (result.success) {
    for (auto& [key, value] : result.metadata) {
        std::cout << key << ": " << value << std::endl;
    }
}
```

### GUI Application (`meta_wiper_gui`)

The GUI is built with Qt 6 and QML, following the Model-View-ViewModel (MVVM) pattern:

- **MVVM Architecture**: Separates data (models), business logic (viewmodels), and presentation (QML views).
- **Models**: `FileListModel` and `MetadataModel` manage file and metadata data.
- **ViewModels**: `MainViewModel` exposes application state and commands to the UI.
- **Application Bridge**: The `Application` class connects QML to the core library and manages workflow.
- **Reactive UI**: Uses Qt's signals/slots and QML property bindings for real-time updates.
- **Asynchronous Operations**: File processing runs in background threads to keep the UI responsive.
- **Component-Based QML**: UI is composed of reusable QML components and a centralized theming system.

```qml
FileList {
    model: fileListModel
    onSelectionChanged: {
        metadataModel.loadMetadata(selectedFilePath)
    }
}
MetadataView {
    model: metadataModel
    onCleanRequested: {
        app.cleanMetadata(selectedKeys)
    }
}
```

### Command-Line Interface (Planned)

A CLI tool will be provided in future releases, leveraging the same core library for batch and scripted metadata operations.

### Data Flow

1. **Selection**: User selects files (via GUI or CLI)
2. **Processing**: Core library processes files and extracts/cleans metadata
3. **Display/Output**: Results are shown in the UI or output to the CLI
4. **Modification**: User selects metadata to clean or overwrite
5. **Saving**: Core library applies changes and saves the modified file(s)

## Project Structure

```
.
├── CMakeLists.txt              # Main CMake configuration file
├── LICENSE                     # GNU GPL v3.0 license file
├── README.md                   # This documentation file
├── cmake/                      # Additional CMake configurations
│   └── CPackConfig.cmake       # Package configuration for distribution
│
├── core/                       # Core library for metadata processing
│   ├── CMakeLists.txt          # Core library build configuration
│   ├── include/                # Public headers
│   │   ├── meta_wipe_core_export.h  # Export macros for library
│   │   ├── meta_wiper_core.h   # Main API header
│   │   ├── base/               # Base components
│   │   │   ├── file_handler.h  # File I/O handling
│   │   │   ├── file_properties.h  # File metadata representation
│   │   │   └── processor_factory.h  # Factory for creating processors
│   │   └── processors/         # Format-specific processors
│   │       ├── docx_processor.h  # DOCX document processor
│   │       ├── jpeg_processor.h  # JPEG image processor
│   │       └── pdf_processor.h   # PDF document processor
│   └── src/                    # Implementation files
│       ├── meta_wiper_core.cpp  # Core API implementation
│       ├── base/               # Base implementations
│       │   ├── file_handler.cpp
│       │   ├── file_properties.cpp
│       │   └── processor_factory.cpp
│       └── processors/         # Format-specific implementations
│           ├── docx_processor.cpp
│           ├── jpeg_processor.cpp
│           └── pdf_processor.cpp
│
├── gui/                        # GUI application
│   ├── CMakeLists.txt          # GUI build configuration
│   ├── include/                # GUI headers
│   │   ├── application.h       # Main application class
│   │   ├── version.h.in        # Version information template
│   │   └── viewmodels/         # ViewModel layer (MVVM pattern)
│   │       ├── filelistmodel.h  # File list management
│   │       ├── mainviewmodel.h  # Main application ViewModel
│   │       └── metadatamodel.h  # Metadata management
│   ├── resources/              # Application resources
│   │   ├── resources.qrc       # Qt resource collection file
│   │   ├── icons/              # Application icons
│   │   │   ├── add.svg         # Add files icon
│   │   │   ├── clean.svg       # Clean icon
│   │   │   ├── clear.svg       # Clear icon
│   │   │   ├── export.svg      # Export icon
│   │   │   ├── info.svg        # Info icon
│   │   │   ├── logo.svg        # Application logo
│   │   │   ├── overwrite.svg   # Overwrite icon
│   │   │   ├── refresh.svg     # Refresh icon
│   │   │   ├── remove.svg      # Remove icon
│   │   │   ├── save.svg        # Save icon
│   │   │   ├── settings.svg    # Settings icon
│   │   │   └── ...             # Other UI icons
│   │   └── qml/                # QML UI files
│   │       ├── main.qml        # Main application window
│   │       ├── components/     # UI components
│   │       │   ├── AboutDialog.qml
│   │       │   ├── ActionPanel.qml
│   │       │   ├── FileList.qml
│   │       │   └── ...         # Other UI components
│   │       └── theme/          # Theming system
│   │           ├── Colors.qml   # Color palette
│   │           ├── Theme.qml    # Theme configuration
│   │           └── Typography.qml  # Text styling
│   └── src/                    # GUI implementation
│       ├── application.cpp     # Application implementation
│       ├── main.cpp            # Entry point
│       └── viewmodels/         # ViewModel implementations
│           ├── filelistmodel.cpp
│           ├── mainviewmodel.cpp
│           └── metadatamodel.cpp
│
└── tests/                      # Test suite
    ├── CMakeLists.txt          # Test build configuration
    ├── main.cpp                # Test entry point
    ├── common/                 # Common test utilities
    │   ├── CMakeLists.txt
    │   ├── test_utils.cpp
    │   └── test_utils.h
    ├── formats/                # Format-specific tests
    │   ├── jpeg/               # JPEG format tests
    │   │   ├── CMakeLists.txt
    │   │   └── jpeg_test.cpp
    │   └── pdf/                # PDF format tests
    │       ├── CMakeLists.txt
    │       └── pdf_test.cpp
    └── resources/              # Test resources
        ├── paths.txt           # Resource paths configuration
        ├── jpeg/               # JPEG test files
        │   ├── test1_metadata.json
        │   ├── test1.jpg
        │   └── test2.jpg
        └── pdf/                # PDF test files
            └── test.pdf
```

This directory structure shows the modular organization of the MetaWiper project:

- The **core** module provides the engine for reading, analyzing, and cleaning metadata
- The **gui** module delivers a modern QML-based user interface
- The **tests** module contains basic functionality tests for the core components

## Testing

The project includes basic tests for the core metadata processing functionality. Currently, tests are implemented as standard C++ applications rather than using a formal testing framework:

```powershell
# Navigate to the build directory
cd build

# Build the tests
cmake --build . --target meta_wiper_tests

# Run tests manually
.\tests\Debug\meta_wiper_tests.exe # For Debug configuration
# For Release configuration, the path would typically be .\tests\Release\meta_wiper_tests.exe
```

> [!NOTE] 
> A more comprehensive testing framework will be implemented in future releases.

## Packaging

The project supports creating installable packages using CPack:

```powershell
# Navigate to the build directory
cd build

# Create installer package
cpack -G NSIS      # Windows NSIS installer
```

## Roadmap

- [ ] Package the GUI application as standalone executable distributions
- [ ] Implement a command-line interface for batch processing and automation
- [ ] Expand support for additional file formats (e.g., PNG, TIFF, PPTX, XLSX)
- [ ] Refactor the core library and generate comprehensive API documentation

## Contributing

> [!IMPORTANT]
> The project is currently undergoing significant changes to its repository structure. Both the API and GUI are not yet stable and will likely undergo major revisions in the near future.

Due to the current state of development, we are not actively seeking code contributions at this time. However, your ideas and feedback are extremely valuable to us:

- **Have suggestions?** Please open an issue to share your thoughts.
- **Found a bug?** Report it via issues with steps to reproduce.
- **Feature requests?** We welcome your ideas to help shape the project's future.

Once the project reaches a more stable state, we will update this section with detailed contribution guidelines.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Contact

💌 **We'd love to hear from you!** 

Have questions, brilliant ideas, or just want to share your experience with MetaWiper? We're all ears! Your feedback is incredibly valuable in making this tool better for everyone.

✨ **Get in touch:**
- Open an [Issue](https://github.com/Richard-Shaun/MetaWiper/issues) - Perfect for bug reports, feature requests, or questions!
- Star the repository if you find it useful - It helps others discover the project!
- Share your success stories and use cases - We're always excited to learn how MetaWiper helps you!

Every comment, suggestion, and question helps shape the future of MetaWiper. Don't hesitate to reach out—we're here to help and eager to build a community around privacy protection!

Project Link: [https://github.com/Richard-Shaun/MetaWiper](https://github.com/Richard-Shaun/MetaWiper)