#include "botpch.h"
#include "../../playerbot.h"
#include "RogueMultipliers.h"
#include "CombatDpsRogueStrategy.h"

using namespace ai;

class CombatDpsRogueStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
	CombatDpsRogueStrategyActionNodeFactory()
	{
		creators["cheap shot"] = &cheap_shot;
		creators["sinister strike"] = &sinister_strike;
		creators["kick"] = &kick;
		creators["kidney shot"] = &kidney_shot;
		creators["rupture"] = &rupture;
		//creators["backstab"] = &backstab;
		creators["melee"] = &melee;
	}
private:
	static ActionNode* melee(PlayerbotAI* ai)
	{
		return new ActionNode("melee",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NextAction::array(0, new NextAction("sinister strike"), NULL));
	}
	static ActionNode* cheap_shot(PlayerbotAI* ai)
	{
		return new ActionNode("cheap shot",
			/*P*/ NextAction::array(0, new NextAction("stealth"), NULL),
			/*A*/ NULL,
			/*C*/ NextAction::array(0, new NextAction("melee"), NULL));
	}
	static ActionNode* sinister_strike(PlayerbotAI* ai)
	{
		return new ActionNode("sinister strike",
			/*P*/ NULL,
			/*A*/ NextAction::array(0, new NextAction("melee"), NULL),
			/*C*/ NULL);
	}
	static ActionNode* kick(PlayerbotAI* ai)
	{
		return new ActionNode("kick",
			/*P*/ NULL,
			/*A*/ NextAction::array(0, new NextAction("kidney shot"), NULL),
			/*C*/ NULL);
	}
	static ActionNode* kidney_shot(PlayerbotAI* ai)
	{
		return new ActionNode("kidney shot",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* rupture(PlayerbotAI* ai)
	{
		return new ActionNode("rupture",
			/*P*/ NULL,
			/*A*/ NextAction::array(0, new NextAction("slice and dice"), NULL),
			/*C*/ NULL);
	}
	static ActionNode* slice_and_dice(PlayerbotAI* ai)
	{
		return new ActionNode("slice and dice",
			/*P*/ NULL,
			/*A*/ NextAction::array(0, new NextAction("rupture"), NULL),
			/*C*/ NULL);
	}

};

CombatDpsRogueStrategy::CombatDpsRogueStrategy(PlayerbotAI* ai) : MeleeCombatStrategy(ai)
{
	actionNodeFactories.Add(new CombatDpsRogueStrategyActionNodeFactory());
}

NextAction** CombatDpsRogueStrategy::getDefaultActions()
{
	return NextAction::array(0, new NextAction("sinister strike", ACTION_NORMAL), NULL);
}

void CombatDpsRogueStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	MeleeCombatStrategy::InitTriggers(triggers);


	triggers.push_back(new TriggerNode(
		"cheap shot open",
		NextAction::array(0, new NextAction("cheap shot", ACTION_NORMAL + 8), NULL)));

	triggers.push_back(new TriggerNode(
		"can open from behind",
		NextAction::array(0, new NextAction("garrote", ACTION_NORMAL + 8), NULL)));

	triggers.push_back(new TriggerNode(
		"combo points available",
		NextAction::array(0, new NextAction("slice and dice", ACTION_HIGH + 2),
			new NextAction("rupture", ACTION_HIGH), NULL)));

	triggers.push_back(new TriggerNode(
		"medium threat",
		NextAction::array(0, new NextAction("vanish", ACTION_HIGH), NULL)));

	triggers.push_back(new TriggerNode(
		"low health",
		NextAction::array(0, new NextAction("evasion", ACTION_EMERGENCY), new NextAction("feint", ACTION_EMERGENCY), NULL)));

	triggers.push_back(new TriggerNode(
		"kick",
		NextAction::array(0, new NextAction("kick", ACTION_INTERRUPT + 2), NULL)));

	triggers.push_back(new TriggerNode(
		"kick on enemy healer",
		NextAction::array(0, new NextAction("kick on enemy healer", ACTION_INTERRUPT + 1), NULL)));

	triggers.push_back(new TriggerNode(
		"light aoe",
		NextAction::array(0, new NextAction("blade flurry", ACTION_HIGH + 3), NULL)));


}
