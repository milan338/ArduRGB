# Contributing to ArduRGB

First and foremost, thanks for taking the the time to contribute. :raised_hands:

The following document is just a set of guidelines for contributing to the project.
These are mostly guidelines, not necessarily rules.
Please feel free to propose any changes to these guidelines in a pull request.

Whether you're reporting a bug, submitting a fix, proposing new features, or just providing feedback, we want to make contributing to this project as easy as possible.

## Code of Conduct

This project and everyone participating in it is goverened by the [code of conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code.
Please make sure to read it prior to contributing.

## Documentation

We use a [GitHub wiki](https://github.com/milan338/ArduRGB/wiki) for documentation.
You can find information on using and developing for this project here.

## We Develop with GitHub

We use GitHub to host code, track issues and feature requests, and accept pull requests.

## We Use [GitHub Flow](https://guides.github.com/introduction/flow/index.html)

Pull requests are the best way to propose codebase changes.
We actively welcome your pull requests:
1. Fork the repo and create your branch from `master`
2. If you've added code that should be tested, add tests and ensure they pass
3. If you've changed APIs, update the documentation
4. Make sure your code follows the style guidelines
5. Submit your pull request

## Contributions Made Will be Under the [Project License](LICENSE)

Whenever you submit code changes, these submissions will be under the same [license](LICENSE) that covers the project.
Please feel free to contact the maintainers if that's a concern.

## Report Bugs and Feature Requests using GitHub's Issue Tracker

We use GitHub issues to track bugs.
Report a bug by opening a new issue.
Remember to select the right template for your issue and to follow it.

## Follow a Consistent Coding Style

This project adheres to [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines) where it does not impact readability or functionality.

Some general guidelines:
- `snake_case` for variable and module names
- `camelCase` for function and method names
- `PascalCase` for class and object names
- Try to adhere to a 79 character character line limit
- Use include guards in header files
  ```c++
  #ifndef PACKAGENAME_H
  #define PACKAGENAME_H
  ...
  #endif // PACKAGENAME_H 
  ```
- Place curly braces on their own line
  ```c++
  void loop()
  {
      ...
  }
  ```

Where readability can be improved, however, exceptions can of course be made.
