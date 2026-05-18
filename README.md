# riscv-as

A tiny and simple RISC-V assembler.

## Building

```bash
$ meson setup build
$ cd build
$ meson compile
```

## Testing

If you want to test the generated binaries with [spike](https://github.com/riscv-software-src/riscv-isa-sim) and/or [pk](https://github.com/riscv-software-src/riscv-pk), you need a RISC-V toolchain.

- Recommended: prebuilt [riscv-gnu-toolchain-builds](https://github.com/FrancescoRocca/riscv-gnu-toolchain-builds).
- Alternative: build the [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) yourself with multilib enabled.

> **Note**: `spike` and `pk` should be built for 32-bit targets and should support RVC.

## Contributing

Please format the code before submitting any contribution. Run `ninja format` inside the `build/` folder.
