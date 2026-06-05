# RV_Project Git Cheat Sheet

## Repository Branches

Permanent branches:

- main → Stable releases only
- develop → Integration branch

Never work directly on main or develop.

---

# Initial Clone

git clone https://github.com/Sief707/RV_Project.git

cd RV_Project

git checkout develop

git pull origin develop

---

# Create New Feature Branch

git checkout -b feature/FEATURE_NAME

Examples:

git checkout -b feature/build-system

git checkout -b feature/image-io

git checkout -b feature/gaussian

git checkout -b feature/sobel

git checkout -b feature/testing

git checkout -b feature/profiling

git checkout -b feature/rvv

---

# Check Status

git status

---

# Check Current Branch

git branch

---

# Show All Branches

git branch -a

---

# Update Develop Branch

git checkout develop

git pull origin develop

---

# Add Changes

Add everything:

git add .

Add specific file:

git add src/filters/gaussian.cpp

---

# Commit Changes

Example:

git commit -m "Implement Gaussian kernel generation"

Good commit messages:

Initialize project architecture

Add host build target

Add RISC-V build target

Implement image loading

Implement Gaussian blur

Implement Sobel gradients

Add Gaussian unit tests

Add profiling framework

Implement RVV Gaussian kernel

Update project documentation

Bad commit messages:

update

fix

final

code

last version

---

# Push Feature Branch

git push origin feature/FEATURE_NAME

Example:

git push origin feature/gaussian

---

# Pull Request Workflow

feature/gaussian
        ↓
Pull Request
        ↓
develop

Never:

feature/*
        ↓
main

---

# Update Feature Branch

git checkout develop

git pull origin develop

git checkout feature/FEATURE_NAME

git merge develop

---

# Switch Branches

git checkout develop

git checkout main

git checkout feature/gaussian

---

# View History

Simple:

git log --oneline

Detailed:

git log --oneline --decorate --graph --all

---

# Restore Uncommitted Changes

Single file:

git restore FILE_NAME

Everything:

git restore .

---

# Delete Branch

Local:

git branch -d feature/FEATURE_NAME

Remote:

git push origin --delete feature/FEATURE_NAME

---

# Daily Team Workflow

1. Checkout develop
2. Pull latest changes
3. Create feature branch
4. Implement feature
5. git add .
6. git commit -m "Meaningful message"
7. git push origin feature/FEATURE_NAME
8. Create Pull Request → develop
9. Review
10. Merge
11. Delete feature branch

---

# Branch Policy

✓ Work on feature branches

✓ Merge feature branches into develop

✓ Merge develop into main at milestones

✗ Never commit directly to main

✗ Never commit directly to develop

✗ Never use meaningless commit messages

---

# Official Team Rule

Every change must follow:

Feature Branch
      ↓
Pull Request
      ↓
Develop
      ↓
Pull Request
      ↓
Main

This workflow is mandatory for all five team members.
