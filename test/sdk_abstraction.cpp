#include <gmock/gmock.h>

#include <sstream>
#include <string>

struct DirectiveSequencerInterface
{
  virtual ~DirectiveSequencerInterface() = default;
  virtual void onDirective(const std::string& directive) const = 0;
};

struct DirectiveSequencer final : public DirectiveSequencerInterface
{                                          // check 2 classi final
  virtual ~DirectiveSequencer() = default; // check 1 distruttore virtuale
  explicit DirectiveSequencer(std::ostringstream& _probe)
    : probe(_probe)
  {
  }
  void onDirective(const std::string& directive) const override
  {
    probe << directive;
  }

private:
  std::ostringstream& probe;
};

// best case scenario
// TODO......... DirectiveSequencer::onDirective(...)

// worst case scenario
struct DirectiveSequencerWrapper : public DirectiveSequencerInterface
{
  explicit DirectiveSequencerWrapper(
    DirectiveSequencerInterface& directiveSequencer)
    : m_directiveSequencer(directiveSequencer)
  {
  }
  void onDirective(const std::string& directive) const override
  {
    // precondition
    m_directiveSequencer.onDirective(directive);
    // postcondition
  }

private:
  DirectiveSequencerInterface& m_directiveSequencer;
};
//===================================================
struct SampleApp
{
  explicit SampleApp(DirectiveSequencerInterface& directiveSequencer)
    : m_directiveSequencer(directiveSequencer)
  {
    m_directiveSequencer.onDirective("directive");
  }

private:
  DirectiveSequencerInterface& m_directiveSequencer;
};

TEST(sdk_abstraction, test)
{
  std::ostringstream probe;
  DirectiveSequencer directiveSequencer(probe);
  DirectiveSequencerWrapper directiveSequencerWrapper(directiveSequencer);
  [[maybe_unused]] SampleApp sampleApp(directiveSequencerWrapper);
  ASSERT_EQ("directive", probe.str());
}
