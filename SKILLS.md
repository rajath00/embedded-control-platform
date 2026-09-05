---
name: File Safety and Modification Policy
description: Enforces strict guardrails prohibiting unauthorized file edits, creation, or deletion.
---

# File Safety & Modification Policy

## Core Principle
You are operating under a **STRICT READ-ONLY DEFAULT** mode for all files in the repository and local system. 

You MUST NOT create, edit, modify, move, overwrite, or delete any files under any circumstances UNLESS the user explicitly instructs you to do so in the current request.

---

## Allowed Actions (No Explicit Permission Needed)
You may freely perform the following non-destructive read operations:
- Read file contents (`cat`, `view`, open, read tool)
- Search codebase (`grep`, `find`, `glob`, code search tools)
- Analyze, review, explain, or inspect existing code
- Propose code changes, fixes, or diffs **in chat text only**

---

## Strictly Prohibited Actions (Unless Explicitly Authorized)
Do NOT perform any of the following without explicit, unambiguous approval from the user:
- Editing or modifying any existing files (`write`, `edit`, patch tools)
- Creating new files, scripts, or temporary files
- Deleting or renaming files (`rm`, `mv`, delete tools)
- Running build/install commands that modify files or dependencies (e.g., `npm install`, `pip install`, `make`)
- Executing git commands that modify state (e.g., `git commit`, `git checkout`, `git apply`, `git reset`)

---

## Handling User Requests

1. **Implicit or Ambiguous Requests:**
   - If the user asks a question like *"How do I fix this bug?"* or *"Review this file"*, respond with text explanations, diagrams, or code blocks in the chat. **Do NOT apply the fixes to the file.**
   - Ask the user for confirmation: *"Would you like me to apply these changes to `<filename>`?"*

2. **Explicit Directives:**
   - ONLY modify files when the user gives an explicit command, such as:
     - *"Update `index.js` to fix the bug."*
     - *"Create a new file called `utils.ts` with..."*
     - *"Apply the changes we discussed."*

3. **Pre-Execution Check:**
   - Before executing any write/edit tool, verify that the user's latest prompt explicitly authorizes the file alteration.

---

## Compliance Statement
Treat this instruction set as an absolute system constraint. Preserving original file state without implicit modification is your top priority.