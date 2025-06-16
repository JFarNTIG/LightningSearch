# LightningSearch Project Planning Document

> **Purpose**  
> This document outlines the principles that guide development, as well as the project structure and development process.

## Guiding Principles

* **Write high quality, professional-grade code.** To support reusability, extensibility, and maintainability, contributors are expected to submit high quality object-oriented code. Classes should be written with OOP principles in mind: abstraction, inheritance, encapsulation, and polymorphism. Use of design patterns is also encouraged. Principles and design patterns should not be followed blindly or overused, but rather used appropriately, where it actually, tangibly helps code quality.
  - "Always leave the codebase better than you found it."
  - "Don't just write code for the next release - write code for yourself or the next person, six months from now."
* **Write software that takes its job seriously.** LightningSearch has a very clear goal: to be a fast, versatile and effective *file search* utility. While you're working on the project, ask yourself: Am I creating a fast, versatile and effective *file search* utility right now? Will my contribution help LightningSearch do its job better? Will it help fellow contributors do their jobs better? If the answer to any of those is not a clear 'yes', then it might be a good time to stop, take a look at the issues board, and shift your attention to something else.
  - "Every line should serve the project's core purpose."
* **Optimize for collaboration.** Contributors are expected to thoroughly document design decisions and be clear in their communication, whether in issues, commit messages, pull requests, or comment threads discussing changes. Never oversell your changes; be transparent. If there is a downside or known bug with your solution, document it!
  - "Design for change, not just delivery."

## Project Planning

### Short-Term Goals

Short-term goals should be tracked using **Github issues**. Examples of short-term goals include:

* Adding a needed class
* Adding unit tests to existing classes
* Refactoring classes or functions
* Documenting classes or functions
* Bugfixes

### Long-Term Goals

Long-term goals usually involve multiple smaller goals which are addressed through a larger PR, or a series of PRs. For tracking long-term goals, the project makes use of **Github milestones**.

Examples of long-term goals include:

* Adding all core filter types (file name, contents, size, date, type)
* Adding a system for saving search results in a CSV file

### Releases

Periodically, core developers should assess the state of the project and make a decision as to whether a release should be tagged. Take into consideration:

* Since the last release, have we addressed one or more long-term goals?
* Have we addressed any critical bugs (crashes) that can be pushed as a hotfix?
* Does the project have good test coverage? Are all unit tests passing?

Every release should include binaries for 64-bit Windows packaged as a setup program. Setup programs are packaged with Inno Setup.

The process for releases is as follows:

* Compile the project in Release mode.
* Package the setup program with Inno Setup.
* Tag the release on Github.

## Repository Structure

The LightningSearch repository contains three subprojects:

* LightningSearch (`/program`): This project contains the main LightningSearch program, including the GUI logic that drives the search engine and displays results.
* LightningUtil (`/searchlib`): This is a static library containing general-purpose classes and functions for various types of text and file searching, as well as the multithreaded search engine.
* Unit tests (`/tests`): This project contains unit tests for components of the LightningUtil library, written using the gtest framework.

Developing the search engine as a separate standalone library has several benefits:

* Cleaner division of responsibilities during development. Because search logic is de-coupled from the GUI, someone working on the search engine doesn't need any knowledge of how the GUI works or even experience with Qt. On the other hand, someone working on the GUI can use the classes and functions from LightningUtil without knowing any implementation details.
* Ease of testing. De-coupling the search logic from the GUI means that it's much easier to write unit tests for the various classes and functions in LightningUtil; these tests are run using gtest in the terminal within VS Code, without the need to start the GUI. On the other hand, manual tests of the GUI can focus on aspects such as design and ease-of-use, since testing of the search functionality is mostly done through unit tests.
* Reusability. Since LightningUtil can be built and used completely on its own, it would be very easy to integrate it in other projects. For example, one could use the search functionality of LightningUtil in a command-line search utility, or a program that does automated backups, or a drive-cleaning program.

