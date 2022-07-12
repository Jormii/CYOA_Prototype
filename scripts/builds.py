from enum import IntEnum


class Attr(IntEnum):
    STRENGTH = 0
    DEXTERITY = 1
    RESILENCE = 2
    AGILITY = 3


class Requirement:

    def __init__(self, attribute, points):
        self.attribute = attribute
        self.points = points

    def __repr__(self) -> str:
        return f"{self.attribute.name} :: {self.points}"


class Skill:

    def __init__(self, name, requirements):
        self.name = name
        self.requirements = requirements

    def __repr__(self):
        return f"{self.name} :: {self.requirements}"


class Build:

    def __init__(self, attr_points, attr_vector, chosen_skills_vector):
        self.attr_points = attr_points
        self.attr_vector = attr_vector
        self.chosen_skills_vector = chosen_skills_vector

    @staticmethod
    def root(level, skills):
        build = Build(
            max(0, 2 * (level - 1)),
            [1] * len(Attr),
            [False] * len(skills)
        )

        return build._invest_to_learn_skills(0, [], skills)

    def knows_skill(self, skill):
        points, _ = self._investment_required(skill)
        return points == 0

    def add_skills(self, skills):
        new_builds = []
        for i, skill in enumerate(skills):
            if self.chosen_skills_vector[i]:
                continue

            points, investments = self._investment_required(skill)
            if points <= self.attr_points:
                new_builds.append(
                    self._invest_to_learn_skills(points, investments, skills))

        return new_builds

    def _investment_required(self, skill):
        points = 0
        investments = []
        for requirement in skill.requirements:
            had = self.attr_vector[requirement.attribute]
            needed = requirement.points

            value = max(0, needed - had)
            points += value
            investments.append(Requirement(requirement.attribute, value))

        return points, investments

    def _invest_to_learn_skills(self, points, investments, skills):
        new_points = self.attr_points - points
        new_attr_vector = list(self.attr_vector)
        new_chosen_skills_vector = list(self.chosen_skills_vector)

        for investment in investments:
            new_attr_vector[investment.attribute] += investment.points

        new_build = Build(new_points, new_attr_vector,
                          new_chosen_skills_vector)

        for i, skill in enumerate(skills):
            if new_build.chosen_skills_vector[i]:
                continue

            if new_build.knows_skill(skill):
                new_build.chosen_skills_vector[i] = True

        return new_build

    def __lt__(self, o):
        if not isinstance(o, Build):
            return False

        return self.attr_points < o.attr_points

    def __repr__(self):
        known = sum(self.chosen_skills_vector)
        return f"{self.attr_points} / {known} :: {self.attr_vector}"


def find_builds(level, skills):
    builds = []
    provisional_builds = [Build.root(level, skills)]
    while len(provisional_builds) != 0:
        build = provisional_builds.pop(0)
        new_builds = build.add_skills(skills)
        if len(new_builds) == 0:
            builds.append(build)
        else:
            provisional_builds.extend(new_builds)

        _delete_same_builds(provisional_builds, _same_attributes)

    builds.sort()
    _delete_same_builds(builds, _same_skills_learnt)
    return builds


def _delete_same_builds(builds, eq_criteria):
    # Delete same builds
    for i in reversed(range(len(builds))):
        for j in range(i):
            if eq_criteria(builds[i], builds[j]):
                del builds[i]
                break


def _same_attributes(build_a, build_b):
    for s1, s2 in zip(build_a.attr_vector, build_b.attr_vector):
        if s1 != s2:
            return False

    return True


def _same_skills_learnt(build_a, build_b):
    for k1, k2 in zip(build_a.chosen_skills_vector, build_b.chosen_skills_vector):
        if k1 != k2:
            return False

    return True


def print_build(build, skills):
    known_skills = {}
    for i, skill in enumerate(skills):
        if build.chosen_skills_vector[i]:
            main_reqr = skill.requirements[0]
            main_attr = main_reqr.attribute

            if main_attr not in known_skills:
                known_skills[main_attr] = []
            known_skills[main_attr].append(skill)

    print(build)
    for attr in Attr:
        attr_skills = known_skills.get(attr, [])

        print(f"-- {attr.name} :: {len(attr_skills)} --")
        for skill in attr_skills:
            print(skill)


if __name__ == "__main__":
    GOAT_SKILLS = [
        Skill("Coz",
              [Requirement(Attr.STRENGTH, 1)]),
        Skill("Concentración",
              [Requirement(Attr.STRENGTH, 2), Requirement(Attr.AGILITY, 2)]),
        Skill("Ariete",
              [Requirement(Attr.STRENGTH, 3), Requirement(Attr.DEXTERITY, 2)]),
        Skill("Consistente",
              [Requirement(Attr.STRENGTH, 4), Requirement(Attr.RESILENCE, 3)]),
        Skill("Diana",
              [Requirement(Attr.STRENGTH, 9)]),
        Skill("Imparable",
              [Requirement(Attr.STRENGTH, 9), Requirement(Attr.DEXTERITY, 5)]),

        Skill("Ojo crítico",
              [Requirement(Attr.DEXTERITY, 1)]),
        Skill("Carga",
              [Requirement(Attr.DEXTERITY, 3)]),
        Skill("Punto débil",
              [Requirement(Attr.DEXTERITY, 5)]),
        Skill("Esteroides",
              [Requirement(Attr.DEXTERITY, 5), Requirement(Attr.STRENGTH, 4)]),
        Skill("Tantear",
              [Requirement(Attr.DEXTERITY, 8), Requirement(Attr.STRENGTH, 5)]),

        Skill("Crecerse",
              [Requirement(Attr.RESILENCE, 1)]),
        Skill("Venganza",
              [Requirement(Attr.RESILENCE, 2), Requirement(Attr.STRENGTH, 2)]),
        Skill("Penetración",
              [Requirement(Attr.RESILENCE, 5), Requirement(Attr.STRENGTH, 2)]),
        Skill("Desesperación",
              [Requirement(Attr.RESILENCE, 9)]),
        Skill("Integridad",
              [Requirement(Attr.RESILENCE, 11)]),

        Skill("Acometer",
              [Requirement(Attr.AGILITY, 1)]),
        Skill("Anticipación",
              [Requirement(Attr.AGILITY, 2), Requirement(Attr.DEXTERITY, 2)]),
        Skill("Contraataque",
              [Requirement(Attr.AGILITY, 3), Requirement(Attr.DEXTERITY, 2)]),
        Skill("Fantasma",
              [Requirement(Attr.AGILITY, 9)]),
        Skill("Inercia",
              [Requirement(Attr.AGILITY, 8), Requirement(Attr.STRENGTH, 5)])
    ]

    level = 10
    skills = GOAT_SKILLS
    builds = find_builds(level, skills)

    print(50 * "-")
    for i, build in enumerate(builds):
        print(f"Build {i+1}")
        print_build(build, skills)
        print(50 * "-")
