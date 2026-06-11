# Phase 2 Development Notes

## Objective

Phase 2 makes the application useful without physical spectrometer hardware by adding realistic synthetic spectra, repeatable datasets, and a file replay backend.

## Changes Made

### Synthetic Spectrum Generator

Need: the previous mock spectrometer embedded a tiny hard-coded two-peak spectrum directly in the device class.

Problem solved: synthetic data can now model multiple sample types, baseline drift, dark current, detector ripple, noise, saturation, integration time, gain, dark frames, and reference spectra. Keeping this in a standalone generator also makes the same spectra reusable for tests, demos, calibration workflows, and future UI controls.

Alternative considered: leave the math inside `MockSpectrometer`. That would be faster short term, but it would make later calibration and regression tests duplicate spectrum-generation logic.

### Named Synthetic Sample Profiles

Need: industrial analysis software needs representative sample conditions, not only one idealized curve.

Problem solved: the generator now supports profiles such as emission standard, absorbance dye, polymer blend, dark frame, and reference lamp. This lets future phases test peak finding, dark subtraction, normalization, and method presets against different spectral shapes.

Alternative considered: use random peaks for every acquisition. That creates visual variety, but it is poor for engineering because results are not repeatable enough for debugging.

### Deterministic Synthetic Sequences

Need: Phase 2 should support repeatable development and future golden-data tests.

Problem solved: synthetic acquisition can produce deterministic sequences from a seed and sequence index. This gives stable spectra while still allowing frame-to-frame variation.

Alternative considered: use `std::random_device` for every frame. That is useful for demos, but it makes regressions harder to reproduce.

### File Replay Spectrometer

Need: without hardware, the app still needs a way to load spectra captured elsewhere.

Problem solved: `FileSpectrometer` can load simple `wavelength,intensity` CSV files and replay them through the same acquisition and processing path as a live device.

Alternative considered: add file loading directly in the UI. That would work initially, but it would bypass the acquisition abstraction and create a second analysis path.

### Sample CSV Fixtures

Need: developers need small known datasets to verify file replay and future processing behavior.

Problem solved: `data/synthetic/emission_standard.csv` and `data/synthetic/dark_frame.csv` provide repeatable fixtures that can be reused by tests, demos, or manual checks.

Alternative considered: generate all fixtures at runtime. Runtime generation is still available, but checked-in CSVs are easier to inspect and useful when validating import/export behavior.

### Phase 2 UI Controls

Need: the synthetic and file replay capabilities should be usable from the application, not only from code.

Problem solved: the UI now exposes acquisition source, synthetic profile, synthetic domain, deterministic seed, file replay path, start/stop state, and acquisition errors. This lets Phase 2 be exercised manually before Phase 3 calibration work begins.

Alternative considered: keep these as hard-coded developer settings until later. That would keep the UI smaller, but it would make calibration workflow development slower because dark/reference/replay conditions would require code edits.

### Controller-Owned Reconfiguration

Need: QML needs to change simulation settings while acquisition runs on a worker thread.

Problem solved: the controller owns configuration changes and restarts the acquisition worker when settings change. QML only calls controller invokables, so thread ownership and backend construction stay in C++.

Alternative considered: expose the worker or mock spectrometer directly to QML. That would reduce a little controller code, but it would leak backend lifecycle details into the UI layer and make later hardware backends harder to swap in.

## Follow-Up Work

- Add unit tests around `SyntheticSpectrumGenerator` and `FileSpectrometer`.
- Add a small command-line or developer utility to export generated spectra to CSV.
- Add richer file parsing for TSV, whitespace-separated data, and headers with units.
