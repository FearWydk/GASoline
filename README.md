[README.md](https://github.com/user-attachments/files/32410487/README.md)
# GASOline

A reusable C++ [Gameplay Ability System](https://docs.unrealengine.com/5.7/en-US/gameplay-ability-system-for-unreal-engine/) (GAS) plugin for Unreal Engine 5. It wraps GAS's setup boilerplate — Ability System Component, Attribute Sets, ability/effect base classes — into a small, reusable core, and grants abilities and attributes to actors through a data asset instead of hand-wiring each one in Blueprint or C++.

GASOline is the combat foundation for **[The Resonance](https://fearwydk.github.io/gamedevportfolio/case-the-resonance.html)**, an elemental action RPG. Case study with more context, screenshots, and a full class-architecture diagram: [fearwydk.github.io/gamedevportfolio/case-gasoline-plugin.html](https://fearwydk.github.io/gamedevportfolio/case-gasoline-plugin.html)

## What it does

- **`UGASO_ASC`** — a thin `UAbilitySystemComponent` subclass with actor-info init handled for you.
- **`UGASO_AttributeSet`** — a base `UAttributeSet` (Health/MaxHealth wired up with `OnRep` callbacks) that game-specific attribute sets extend.
- **`UGASO_AbilityBase`** / **`UGASO_EffectBase`** — base classes every gameplay ability and gameplay effect in a project built on GASOline extends.
- **`UGASO_AbilitySet`** — a `UPrimaryDataAsset` that lists the abilities, effects, and attribute sets an actor should be granted, and grants them in one call:

  ```cpp
  void UGASO_AbilitySet::GiveToAbilitySystem(
      UGASO_ASC* GASOASC,
      FGASO_AbilitySet_GrantedHandles* OutGrantedHandles,
      UObject* SourceObject) const;
  ```

  Handles are tracked as they're granted, so a set can be cleanly revoked later — swap an actor's kit at runtime without leaking abilities or effects.

- **`AGASO_CharacterBase`** — an `ACharacter` base that owns the ASC and AttributeSet and implements `IAbilitySystemInterface`, so a new character only needs to assign an Ability Set asset to get a working kit.

Everything above is game-agnostic. The Resonance builds on top of it with its own `YH_`-prefixed layer — player/enemy attribute sets, six combat abilities (sword, gun, finisher, hit react, death, dash), and matching gameplay effects — without touching the plugin itself.

## Why a data asset instead of hardcoding grants

Designers can build a new enemy or player loadout by creating a `GASO_AbilitySet` asset and dropping abilities into an array — no C++ or Blueprint graph editing required. The same asset type covers wildly different actors: a player kit with five abilities and an enemy kit with three both use the exact same data structure, same granting code path.

## Status

Actively developed alongside The Resonance. Currently evaluating a refactor to move The Resonance's game-specific attribute sets out of the plugin entirely, so GASOline becomes fully game-agnostic and drop-in for future UE5 projects.

## Requirements

- Unreal Engine 5.7+
- Enable the built-in **Gameplay Abilities** plugin (GASOline builds on top of it, not instead of it)

## License

See [LICENSE.md](LICENSE.md).
