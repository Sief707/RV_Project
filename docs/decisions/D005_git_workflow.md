# D005 – Git Workflow and Collaboration Strategy

## Purpose

This document defines the official Git workflow for the RV_Project team.

The goal is to:
- Maintain a clean repository history.
- Enable safe collaboration between all team members.
- Satisfy the project requirements regarding Git usage, branches, commits, and pull requests.
- Ensure all work is traceable and reviewable.

---

# Repository Branch Structure

The repository contains two permanent branches:

main
develop

## main

The main branch always represents the most stable version of the project.

Rules:

- Never commit directly to main.
- Never develop new features on main.
- Only tested and reviewed code reaches main.
- Milestone releases are merged into main.

## develop

The develop branch is the integration branch.

Rules:

- Feature branches are merged into develop.
- Daily development is integrated here.
- Team testing is performed here before releasing to main.

---

# Feature Branch Workflow

Every task must be implemented in a dedicated feature branch.

Examples:

- feature/build-system
- feature/image-io
- feature/gaussian
- feature/sobel
- feature/testing
- feature/profiling
- feature/rvv
- feature/documentation

No work should be performed directly on:

- main
- develop

---

# Starting New Work

Before beginning a task:

git checkout develop
git pull origin develop

Create a new branch:

git checkout -b feature/gaussian

Work only inside the created branch.

---

# Commit Guidelines

Each commit should represent a single logical change.

Good examples:

- Implement Gaussian kernel generation
- Add image loading interface
- Implement Sobel gradient computation
- Add benchmark timing utility
- Add QEMU execution script
- Implement RVV Gaussian vectorization

Bad examples:

- update
- fix
- final
- last version
- code

Commit messages must clearly describe what changed.

---

# Local Development Cycle

After making changes:

git add .
git commit -m "Implement Gaussian kernel generation"

Push the branch:

git push origin feature/gaussian

---

# Pull Request Workflow

After completing a feature:

feature/gaussian
      ↓
Pull Request
      ↓
develop

Rules:

- Never merge directly into main.
- Every feature must be submitted through a Pull Request.
- Pull Requests should contain a clear description of the completed work.
- Documentation updates must be included when relevant.

---

# Code Review Policy

Before merging:

- Build must succeed.
- Tests must pass.
- Documentation must be updated.
- At least one teammate reviews the Pull Request.

Checklist:

[ ] Code builds successfully
[ ] Tests pass
[ ] Documentation updated
[ ] No merge conflicts
[ ] Reviewed by teammate

---

# Releasing to Main

When a project milestone is complete:

Example:

- Image I/O
- Gaussian Blur
- Sobel Operator
- Unit Tests

All features are merged into develop.

Then a final Pull Request is created:

develop
    ↓
Pull Request
    ↓
main

This creates a stable milestone release.

---

# Team Responsibilities

Suggested distribution:

| Member | Responsibility |
|----------|----------|
| Member 1 | Build System and Integration |
| Member 2 | Image I/O |
| Member 3 | Gaussian Blur |
| Member 4 | Sobel and Gradient Processing |
| Member 5 | Testing, Benchmarking, and Documentation |

All members must follow the same Git workflow.

---

# Branch Protection Rules

Recommended repository policy:

- No direct commits to main.
- No direct commits to develop.
- All changes through Pull Requests.

---

# Expected Git History

Good history example:

- Initialize project architecture
- Add host build target
- Add RISC-V build target
- Implement image loading
- Implement Gaussian blur
- Add Gaussian unit tests
- Implement Sobel gradients
- Add profiling framework
- Implement RVV Gaussian kernel

Poor history example:

- update
- update2
- fix
- final
- lastversion
- finalfinal

The repository history should clearly demonstrate project progress.

---

# Daily Workflow Summary

1. Pull latest develop
2. Create feature branch
3. Implement feature
4. Commit logical changes
5. Push branch
6. Create Pull Request
7. Review
8. Merge into develop
9. Release develop → main at milestones

---

# Official Team Rule

- Never work directly on main.
- Never commit directly to develop.
- Always use feature branches.
- Always use Pull Requests.
- Always write meaningful commit messages.
- Always keep documentation updated.

Following this workflow ensures clean collaboration, reproducible development, and compliance with project requirements.
